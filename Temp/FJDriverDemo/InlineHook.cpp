#include "stdafx.h"
#include "KernelHookAPI.h"
#include "InlineHook.h"
#ifdef FDRIVER_DEMO_PROJECT
#  include "FDriverUtil.h"
#endif 

#include "KernelHelper.h"

#define INLINE_HOOK_TAG				'HOOK'

#if defined(_M_IX86)
  typedef LONG						POINTER_TYPE;
#elif defined(_M_X64)
  typedef LONGLONG					POINTER_TYPE;
#endif

#pragma pack(push, 1)
struct JMP_x86_OFFSET
{
	UCHAR		opcode[1];
	ULONG		operand;
	VOID SetJmpTarget(POINTER_TYPE target, POINTER_TYPE from)
	{
        //32λ�������ת
        opcode[0] = 0xE9;	//jmp operand
		operand = (target - from - sizeof(*this));
	}
};
struct JMP_x64_ABS
{
	UCHAR		opcode[2];
	LONG		offsetAddr;
	ULONGLONG	operand;
	VOID SetJmpTarget(POINTER_TYPE target, POINTER_TYPE /*from*/)
	{
        //64λ�Ǿ�����ת
		opcode[0] = 0xff; opcode[1] = 0x25;			//jmp [+imm32]
		offsetAddr = 0;								//must zero, 
		operand = target;
	}
};
#pragma pack(pop)

#if defined(_M_IX86)
#  define INSTRUCTION_LENGTH_TYPE	0
#  define GetInstructionLength		GetInstructionLength_x86
#  define JUMP_CODE_TYPE			JMP_x86_OFFSET

EXTERN_C int __stdcall				GetInstructionLength_x86(void* InPtr, int InType);
#elif defined(_M_X64)
#  define INSTRUCTION_LENGTH_TYPE	64
#  define GetInstructionLength		GetInstructionLength_x64
#  define JUMP_CODE_TYPE			JMP_x64_ABS

EXTERN_C int __stdcall				GetInstructionLength_x64(void* InPtr, int InType);
#endif 


ULONG LhRoundToNextInstruction(void* InCodePtr, ULONG InCodeSize);


NTSTATUS CreateInlineHook(PVOID pTarget, PVOID const pDetour, PVOID* ppOriginal, PINLINE_HOOK_INFO* ppOutHookInfo)
{
	NTSTATUS status = STATUS_SUCCESS;
	LONG           			EntrySize;
	LONG					nHookJmpSize = sizeof(JUMP_CODE_TYPE);
	//LONG					popSize;
	//POINTER_TYPE				RelAddr;

//#if defined(_M_IX86)
//	UCHAR			            Jumper[12] = {0xE9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//#elif defined(_M_X64)
//	UCHAR			            Jumper[12] = {0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0};
//#endif 

	EntrySize = LhRoundToNextInstruction(pTarget, nHookJmpSize); //JUMPER_SIZE);
	NT_ASSERT(EntrySize + nHookJmpSize < MAX_REPLACE_CODE_SIZE);


	PINLINE_HOOK_INFO pHookInfo = (PINLINE_HOOK_INFO)ExAllocatePoolWithTag(NonPagedPool, sizeof(INLINE_HOOK_INFO), INLINE_HOOK_TAG);
	if (pHookInfo)
	{
		JUMP_CODE_TYPE	jmpType;

		RtlZeroMemory(pHookInfo, sizeof(INLINE_HOOK_INFO));
		pHookInfo->pTarget = (PBYTE)pTarget;
		pHookInfo->pDetour = (PBYTE)pDetour;
		pHookInfo->pOriginal = pHookInfo->trampoline;


		RtlCopyMemory(pHookInfo->targetBackup, pTarget, EntrySize);
		pHookInfo->targetBackupSize = EntrySize;

		pHookInfo->trampolineSize = EntrySize;
		RtlCopyMemory(pHookInfo->trampoline, pTarget, EntrySize);

        //���� trampoline ���� -- ���� Target ��ָ��λ��
		jmpType.SetJmpTarget((POINTER_TYPE)((PBYTE)(pHookInfo->pTarget) + nHookJmpSize), (POINTER_TYPE)pHookInfo->trampoline + EntrySize);
        RtlCopyMemory(pHookInfo->trampoline + EntrySize, &jmpType, nHookJmpSize);

		//���� target ����, ��ת���Զ���Detour(Hook) ����
		jmpType.SetJmpTarget((POINTER_TYPE)((PBYTE)(pHookInfo->pDetour)), (POINTER_TYPE)pHookInfo->pTarget);

        KIRQL irql = ClearWriteProtect();
        RtlFillMemory(pHookInfo->pTarget, EntrySize, 0x90);     //fill nop
		RtlCopyMemory(pHookInfo->pTarget, &jmpType, nHookJmpSize);
		RestoreWriteProtect(irql);

		if (ppOriginal)
		{
			*ppOriginal = pHookInfo->pOriginal;
		}
		*ppOutHookInfo = pHookInfo;
	}
	return status;
}

NTSTATUS RestoreInlineHook(PINLINE_HOOK_INFO pHookInfo)
{
	NTSTATUS status = STATUS_SUCCESS;

	KIRQL irql = ClearWriteProtect();
	RtlCopyMemory(pHookInfo->pTarget, pHookInfo->targetBackup, pHookInfo->targetBackupSize);
	RestoreWriteProtect(irql);

	ExFreePoolWithTag(pHookInfo, INLINE_HOOK_TAG);

	return status;
}

ULONG LhRoundToNextInstruction(
			void* InCodePtr,
			ULONG InCodeSize)
{
	UCHAR*				Ptr = (UCHAR*)InCodePtr;
	UCHAR*				BasePtr = Ptr;
    LONG                nSize;

	while(BasePtr + InCodeSize > Ptr)
	{
		nSize = GetInstructionLength(Ptr, INSTRUCTION_LENGTH_TYPE);
		if (nSize <= 0)
		{
			return nSize;
		}
		Ptr += nSize;
	}

	return (ULONG)(Ptr - BasePtr);
}