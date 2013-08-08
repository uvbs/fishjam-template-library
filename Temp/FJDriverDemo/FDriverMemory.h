#ifndef F_DRIVER_MEMORY_H
#define F_DRIVER_MEMORY_H

/******************************************************************************************************************
* Windows���������У����г���ĺ����ͱ�����Ҫָ�������ص� ��ҳ�ڴ� �л��� �Ƿ�ҳ�ڴ� ��
*   ��������ж������� DISPATCH_LEVEL ������ʱ������ֻ��ʹ�÷Ƿ�ҳ�ڴ棬���򽫵�������������
*
*   ��ҳ�ڴ� -- ���Խ������ļ��е������ڴ�ҳ��
*   �Ƿ�ҳ�ڴ� -- ��Զ���ᱻ�������ļ��е������ڴ�ҳ�棬
*   INIT��־���ڴ� -- һ���DriverEntry����ֱ�ӵ��õĸ�������ʹ�ã������ú���ֻ�ڼ��ص�ʱ����Ҫ�����ڴ棬����������ɹ����غ󣬸ú������Դ��ڴ���ж�ص�
*
* �����ڴ��ַ(Physical Memory Address) -- �Կ��ȵ��Դ��ӳ�䵽ʵ�ʵ������ڴ��ַ
* �����ڴ��ַ(Virtual Memory Address) -- 
* 
* ����������Ĳ�ͬ���������ڲ�ͬ�Ľ����У�
*   1.DriverEntry �� AddDevice ������������ϵͳ(System)������
*   2.����ǲ������������Ӧ�ó�����������У��� IRP_MJ_READ��IRP_MJ_WRITE ��
*
* ���������ջ�ռ��С���������������ʺϵݹ���û�ֲ������Ǵ��ͽṹ�壬����������Ҫ�ڶ������롣
* Ƶ������ͻ����ڴ棬�ᵼ�����ڴ��ϲ����������ڴ桰�ն������Ӷ����������޷������ڴ棬DDK�ṩ�� Lookaside �ṹ�����������⡣
*
* ��ָ̽���ڴ�Ŀɶ�д�� -- ���ɶ�дʱ�������쳣�������ýṹ���쳣����(__try...__except)
*   ProbeForRead/ProbeForWrite
******************************************************************************************************************/

//��ҳ�ڴ棬�������ڷ�ҳ�ڴ���ʱ����Ҫ�ں�����ʵ���м��� PAGED_CODE() �����ú����Ƿ����е��ж����󼶣�������Ļ����������
#define PAGEDCODE   code_seg("PAGE")

//�Ƿ�ҳ�ڴ�
#define LOCKEDCODE  code_seg()

//ָ���˺����Ǽ��ص�INIT�ڴ������У����ɹ����غ󣬿����˳��ڴ�(DriverEntry)
#define INITCODE    code_seg("INIT")            

#define PAGEDDATA   data_seg("PAGE")
#define LOCKEDDATA  data_seg()
#define INITDATA    data_seg("INIT")

//�������ƵĹ��ܣ�
#if 0
//#ifdef ALLOC_PRAGMA
//#  pragma alloc_text(INIT, DriverEntry)
//#  pragma alloc_text(PAGE, TestDriverDispatchCreate)
//#endif
#endif 

#endif //F_DRIVER_MEMORY_H