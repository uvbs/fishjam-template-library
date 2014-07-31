package com.fishjam.study.javaee;
/**************************************************************************************************************************************
 * http://www.cnblogs.com/ycxyyzw/archive/2012/10/13/2722567.html
 * 
 * Ibatis(2010 改名为 MyBatis) -- 基于SQL映射支持Java(SQL Maps)和·NET(DAO)的持久层框架， 类似 Hibernate(全自动映射)，
 *   但程序员需要编写具体的SQL，然后通过映射配置文件，将SQL所需的参数，以及返回的结果字段映射到指定 POJO。
 *   ibatis 以 SQL开发的工作量和数据库移植性上的让步，为系统设计提供了更大的自由空间。
 *
 * 简单的SQL语句可以用 @Select 直接写在Java源码里
 * 较复杂的SQL语句需要写在 .xml 配置文件里(这样以后更改了 SQL语句后，就不需要去改java代码了 )
 * 		<typeAlias alias="Student" type="com.iflytek.entity.Student" />  //
 * 		<select id="selectAllStudent" resultClass="Student">  //id表示select里的sql语句，resultClass表示返回结果的类型
 *			select * from 	tbl_student
 *		</select>
 *    
**************************************************************************************************************************************/

public class IbatisStudy {
}