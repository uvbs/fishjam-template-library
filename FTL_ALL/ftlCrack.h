
#ifndef FTL_CRACK_H
#define FTL_CRACK_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlCrack.h requires ftlbase.h to be included first
#endif

/*********************************************************************************************************************************
* Ȩ�޼�鹤�ߣ�AccessChk -- ��֪��ָ���˺Ŷ��ض�Ŀ¼��Ȩ�޺������Լ����
*   AccessChk -i Ŀ¼ -- �鿴Ŀ¼���ļ��������Լ���
*   AccessChk -d -q %AppData% 
*********************************************************************************************************************************/

/*********************************************************************************************************************************
* ��ȫ����
*   Security Principal -- Windows���εİ�װ��ȫ����
* 
*   DEP -- ����ִ�б���
*   MIC(Mandatory Integrity Control) -- ǿ�������Կ��ơ�Vista�е����а�ȫ�Զ���ͽ��̶���һ�������Լ���
*     �����Լ���(Integrity Level -- IL)�͵Ľ��̲����޸�(���Զ�ȡ?)����ߵ��ļ���ע������
*     ע�⣺Win2K/XP �£���ȫ��ϵͳֻ�ѽ��̵ķ������ƺ���Դ�ķ��ʿ����б�����ƥ��Ƚϣ���ȷ�ϸý����Ƿ���з��ʸ���Դ��Ȩ�ޡ�
*     1.�����μ��� -- �����ø����������ӻỰ
*     2.Low(��) -- ����ģʽ�µ�IE�����, �� Untrusted ��ϵͳȨ�����г���ֻ�ܴ�ȡ�ʹ�ȡ�����·��λ�ã��� "Temporary Internet Files\Low"��
*       ע�⣺δ���û�ͬ������ص���Ϊ�����ڵͼ���
*             �û��ֶ������ػ�����ִ�м���һ������ �������еȼ���
*             ��һ������Ȩ�޵��û���ͬ�������(���簲װһ��ActiveX�ؼ�)�������ڸ������Լ���
*     3.Medium(�е�) -- Ĭ������£��û�����Ĵ��룬��Windows Explorer�����������, ʹ��"User"Ȩ�ޣ��ܶ�д�û����ļ���ע�����
* ����4.High(��) -- ������ administrator�û� ������Ȩ�޺�ĳ���
* ����5.System(ϵͳ) -- �ں˼����Windows�ļ�
*     6.�������̼��� -- ������ߣ�ֻ����ϵͳ��Ҫ��ʱ��Żᱻʹ��
*     
*   Ӧ���ļ���ע�����⻯(�����ض���) -- ��ָ��λ�ý��ж�д��ʱ��ᱻ�ض���ÿ���û������⻯������
*     ������λ�ý�����һ����"�û������ļ���"��ȫһ�µ��������Լ���Ϊ"Low"��Ŀ¼���:
*     %UserProfile%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\XXXX -- XXX���ּ���Ӧ�õ� C:\ProgramData\MyProgramData ��
*
*     ����ע������п��ƣ���HKLM\SYSTEM\CurrentControlSet\services\luafv
*   SFP -- ϵͳ�ļ�����, Win2K ��ǰ���ļ���������
*   UAC(User Account Control) -- �û��ʻ����ƣ� �ù���Ա�ʻ��Զ����һ����׼�û��ķ������ƣ��Լ���Windows Vistaϵͳ���ܹ�����
*   UIPI(User Interface Privilege Isolation) -- �û�������Ȩ���룬�����Լ���͵Ľ��̣������������Լ���ߵĽ��̷���Window��Ϣ��
*   WFP -- Windows�ļ�����, Win2Kʱ���룬ֻ�����ļ�
*   WRP -- Windows��Դ�������Թؼ��ļ����ļ��к�ע���ֵ���б���
*
*
*********************************************************************************************************************************/

/*********************************************************************************************************************************
* ������ת��ʮ���ƣ�������������ÿһλ����������Ȩ��Ȼ����ӡ�
*   (100110.101)2 = 1*2^5 + 1*2^2 + 1*2^1 + 1*2^(-1) + 1*2^(-3) = 32+4+2+0.5+0.125 = 38.625
* ʮ����ת�������ƣ���������(��2ȡ��)��С������(��2ȡ��)�ֱ�ת����Ȼ���ٺϲ���
*   �� д�ɰ���������Ȩ�Ĵ�Сչ���Ķ���ʽ����Ȩֵ�Ӹߵ�������ȡ�����ϵ����
*   (175.71875)10 = 128+32+8+4+2+1+0.5+0.125+0.0625+0.3125 
*                 = 2^7+2^5+2^3+2^2+2^1+2^0+2^(-1)+2^(-3)+2^(-4)+2^(-5)
*                 = (10101111.10111)2
*   
* ԭ�룺���λ�Ƿ���λ(0��ʾ���ţ�1��ʾ����)�������n-1λ��ʾ��ֵ�ľ���ֵ��
*   0��ԭ���ʾ�����֣�[0]ԭ = 0 000 0000, [-0]ԭ = 1 000 0000
* ���룺���λ�Ƿ���λ(0��ʾ���ţ�1��ʾ����)�������ķ����ԭ����ͬ�������ķ������������ֵ��λ�󷴡�
*   0�ķ����ʾ�����֣�[0]�� = 0 000 0000, [-0]�� = 1 111 1111
* ���룺���λ�Ƿ���λ(0��ʾ���ţ�1��ʾ����)�������Ĳ����ԭ����ͬ�������Ĳ���������䷴��+1��
*   0��Ψһ�Ĳ��� [+0]��=[-0]��=0 000 0000�����ò��룬���Խ���������ת��Ϊ�ӷ����㡣
* ���룺����X������һ��ƫ�����������ڱ�ʾ�������еĽ��롣��������ֳ�Ϊn����ƫ��2^(n-1)������£�
*   ������ķ���λȡ���Ϳɻ����Ӧ�������ʾ��Ҳ������ԭ��+127,1023��16383��
*   [-45]ԭ=1 010 1101�� [-45]��=1 101 0010�� [-45]��=1 101 0011��[-45]��=0 101 0011
* 
* ģ2����Ӽ�������λ���㣬��������λ�ͽ�λ��
* 
* ��������С�����λ�ù̶����������ͨ�������֣��������� �� ����С�����ܱ�ʾ����ֵ��Χ��С��
* ��������С�����λ�ò��̶��������ܱ�ʾ����Χ��ʹ�ý����β������ʾ����������N = 2^E * F (����E��Ϊ ���룬 F��Ϊ β��)
*   ��ʾ��ʽΪ���׷�|����|����|β�������ʾ��Ψһ���������ܱ�ʾ����ֵ��Χ�ɽ������������ʾ��ֵ�ľ�����β��������
*   Ϊ�˳������β������ʾ�������Ч���֣�ͨ�����ù�񻯸�����(��β���ľ���ֵ�޶���[0.5,1]��������)��
*   ͨ�������������ʾ��β����ԭ���ʾ��IEEE 754 ��׼�жԸ�������С���� ָ�����ȹ涨�ֱ��ǣ�float(8),double(11),���侫��(15)
*   ��IEEE 754��׼�� 176.0625 ��ʾΪ�����ȸ�������(176.0625)10 = (1011 0000.0001)2 = 1.011 0000 0001 * 2^7 
*     =>(��С��������չΪ�����ȸ��������涨��23λβ����˫����Ϊ52,���侫��Ϊ64) 011 0000 0001 0000 0000 0000
*     =>(������룬ǰ��ָ��Ϊ7�����ϵ�����ƫ�� 127��˫����Ϊ1023�����侫��Ϊ16383)E=7+127=134,��ָ���������ʾΪ 1000 0110
*     ���ɵã�(176.0625)10 �ĵ����ȸ�������ʾ��ʽΪ 0 10000110 011 0000 0001 0000 0000 0000(0x43301000)
*     float f = 176.0625;
*     DWORD d = *((DWORD*)(&f));
*     CPPUNIT_ASSERT(d == 0x43301000);
* 
* ���ֱ��룺
*   ���Ļ������Ž��٣���������(ASCII)���ڼ����ϵͳ�У����롢�ڲ��������洢�����������ͬһ���루���Ӧ��Ҳ�������룿����
*   �������෱�࣬���롢�ڲ��������洢������Ժ��ִ����Ҫ�󲻾���ͬ�����õı���Ҳ��ͬ��
*     �����룺�������ı�׼�������뺺��ʱ�ı��룬���õ������ࣺ���ֱ���(�������λ��--94x94)��ƴ����(�Ժ������Ϊ����)��������(�Ժ��ֵ���״ȷ���������)
*     �ڲ��룺�������豸����Ϣ����ϵͳ�ڲ��ı�����ʽ���Ǵ洢�������������õĴ��롣��GB2312�ĺ��ֹ����룬��GB2312�����ֽڵĸ�λ��1��
*       �͵õ��˻�����(GBK�����ASCII��)���硰�󡱵�GB2312Ϊ3473H��������ΪB4F3H
*       UCS(Universal Character Set,Ҳ��ISO 10646) -- Ϊ��ͳһ�ر�ʾ������������֣�Ϊ�������պ��ȹ��������ڵ�
*         ��������ʹ�õ����ֹ涨��ͳһ�ı��뷽������4���ֽ�ָ�� �顢ƽ�桢�к���λ( 2^7 x 2^8 x 2^8 x 2^8)��
*         Unicode ��׼��Ӧ�� ISO 10646 ʵ�ּ��� 3(��֧�����е� UCS ����ַ�)��
*         UTF-8 �� UCS ���θ�ʽ 8����һ�ּ����� ASCII ��������� POSIX �ļ�ϵͳ�Ŀɱ䳤����
*     �����룺���ں����������ʾ�������ε���ģ���ݣ�ͨ���õ���ʸ�������ȷ�ʽ��ʾ������ʾ���ӡ���ʱ�������ֿ⣬�����ģ����õ����Ρ�
*       ���������Ҫ��ͬ������Ķ���Ҳ��ͬ���磺�����͵� 16x16���󣬸߾����͵� 24x24����32x32����48x48����ȡ�
*       ʸ����ʾ���ǽ����ֿ����ɱʻ���ɵ�ͼ�Σ���ȡÿ���ʻ�������ֵ����ÿһ�����ֵ���������ֵ����������Ǹú������ε�ʸ����Ϣ��
*
* У���� -- �����ݿ��ܳ��ֵı����Ϊ���ࣺ�Ϸ�����ʹ�����롣�Ϸ��������ڴ������ݣ���������ǲ������������г��ֵı��롣
*   ��� -- һ������ϵͳ�����������Ϸ�����֮�������ж��ٸ�������λ��ͬ,���Ϊ1ʱ�޲�����ܣ���8421�룩
*   ���õ�У���룺
*   1.��żУ����(parity code)���ڱ���������һλУ��λ��ʹ������1�ĸ���Ϊ����(��У��)��ż��(żУ��)�����Ϊ2���ܷ��ִ��󣬵�����У������
*   2.������(hamming code)��Ҳ��������ż��������;�����������λ֮�����k��У��λ��ͨ�����������ʵ�ּ���;������� 8λ���� �в��� 4λУ�顣
*   3.CRC���㷺Ӧ��������ͨ������ʹ洢ϵͳ�С��������ɶ���ʽΪk������λ����r��У��λ�����б��룬����볤��Ϊk+r�������n�����ݣ�������
*      k����Ϣ�룬��У����ռ n - k ������Ϊ (n ,k)�롣У��������Ϣ���������λ��Խ����У��������Խǿ������CRC����ʱ�����õ���ģ2���㡣
*
* �߼�����
*   ��(�߼���) -- AND
*   ��(�߼���) -- OR
*   ��(�߼���) -- NOT
*   ���(���) -- XOR
*   

**********************************************************************************************************************************/
/**********************************************************************************************************************************
* ��Կ/˽Կ�� -- ʹ�õ���������ѧԭ��������Կ���ܺ�ֻ����˽Կ���ܣ�˽Կ���ܺ�ֻ���ù�Կ����
* 
* �Գ���Կ��˽Կ���ܺϽ��ܣ��ٶȿ죬����Կ��������
* �ǶԳ���Կ����Կ���ܣ�˽Կ���ܣ����������ٶ�����
*   ���ܷ��ͣ�������ʹ�ý����ߵĹ�Կ���ܲ����ͣ�һ�����ܣ�ֻ���ɽ��ܷ���˽Կ����
*   ����ǩ�����û����Լ���˽Կ���ܣ���ͨ���乫Կ���ܽ�����֤
*   
* ����֤�飺�й�Կ��Ϣ���Ӷ�ȷ����ӵ����Կ�Ե��û�������
* ��Կλ�� ~/.ssh/ Ŀ¼�У���Ŀ¼���������ƣ������˻������������˻�(����root)��ֻ���������޷���¼����������ʱ���� sshd_config ����
*   �ļ����� StrictMode no ��ȡ��������(man sshd_config)
*
* AES(Advanced Encryption Standard) -- �߼����ܱ�׼����Rigndael���ܷ����������ԭ�ȵ�DES
**********************************************************************************************************************************/
namespace FTL
{
    class CFCrackUtility
    {
    public:
        enum CrackStringType
        {
            //csLowerCase,
            //csUpperCase,
            //csNumber,
            csKiloString,
            //csPlaceString,
        };
        FTLINLINE CFCrackUtility();
        FTLINLINE virtual ~CFCrackUtility();

        FTLINLINE LPCTSTR GetCrackString(CrackStringType csType,DWORD dwPlaceStart = 0);
    private:
        LPTSTR  m_pszCrackString;
    private:
        static LPCTSTR s_csKiloString;
        static LPCTSTR s_csPlaceString;
    };
}

#endif //FTL_CRACK_H



#ifndef USE_EXPORT
#  include "ftlCrack.hpp"
#endif