package com.fishjam.storeserver.study;

/**************************************************************************************************************************************
 * TODO:
 *   1.����ͻ��˹���ͬһ��Servletʵ����
 *   2.ÿ��������һ��Java�̴߳���?
 *
 * ע�⣺
 *   1.Http�ж�������ʽ(GET/POST/PUT ��),HttpServlet�ж�Ӧ�ṩ�� doGet/doPost/doPut �ȷ��񷽷��ֱ��Ӧ��
 *     ������봦���������͵�����ֻ��򵥵�ʵ�� service ��������
 *     TODO:��������Ļ������ṩ���� doXxx ������ʲô��
 *   
 * Servlet(Server Applet) -- �ڷ����������е�С����,��Ҫ���ڽ���ʽ��������޸����ݣ����ɶ�̬Web����
 *   Servlet�������𴴽�HttpServlet���󣬲���Http����ֱ�ӷ�װ��HttpServlet������
 
 *
 * javax.servlet.http.HttpServlet��
 *   init() -- �����ʼ��Servlet����
 *   service() -- ���ģ�������Ӧ�ͻ�������super.service()�������ܼ������������Դ���������doGet()��doPost()��
 *    a.ͨ�� Request ��������� ��������(getParameter)��URL�ϵĲ�ѯ�ַ���
 *    b.ͨ�� Response ������Ӧ����, getWriter ����ı���Ϣ��getOutputStream() �����������Ϣ
 *   destory() -- �����ͷ�ռ�е���Դ
 * 
 * HttpServletRequest
 *   getParameter -- 
 * HttpServletResponse
 *   setContentType("text/html;charset=GB2312");
 * 
 * TODO: 
 *   
**************************************************************************************************************************************/

public class ServletStudy {

}