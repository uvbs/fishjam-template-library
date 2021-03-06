package com.fishjam.spring.test.framework;

import org.junit.Test;

import junit.framework.TestCase;

/*******************************************************************************************************************************************************************
 * Spring表达式语言(Spring Expression Language -- SpEL) : 通过运行期执行的表达式将值装配到Bean的属性或构造器参数中。
 *   可以在定义Bean的XML中进行配置，也可以通过 @Value("#{表达式}") 的方式通过注解在代码中使用
 *    注意：虽然功能很强大， 但是没有IDE的语法检查支持，很难调试和维护，因此不要把过多的逻辑放入SpEL表达式中
 *    语法：value = "#{表达式}"， 
 *       #{myBean.property1}  <== 获取属性值 
 *       #{myBean.getObj()?.fun()}<== 调用getObj()方法获得返回的对象，如不为null，则通过其fun()方法获得返回值
 *       T() <== 调用类作用域的方法和常量，通常可用于调用静态方法和常量， #{T(java.lang.Math).PI * circle.radius ^ 2 } 计算园的面积
 *          <property name="randomNumber" value="#{java.lang.Math}.random()}" /> -- 配置随机数变量(0到1之间)到Bean的一个属性中
 *       运算符的文本替代方式(textual alternatives): ==(eq), <(lt), <=(le), >(gt), >=(ge)
 *       逻辑表达式<== and / or / not(或 ! ),  true, false -- TODO: 似乎没有 & 和 | 
 *       条件表达式<==  条件 ? 真时的值 : 假时的值， 常见的使用场景为检测null， 简化形式为 #{ 变量 ?: "默认值" }   
 *       正则表达式匹配(matches) <== <property name="validEmail" value="#{admin.email matches '[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.com'}" /> 
 *       操作集合 <== 获取一个成员: #{集合名[index]};  或 #{集合名['keyName']}。
 *          特殊：#{systemEnvironment['JAVA_HOME']} -- 访问名为 JAVA_HOME 的环境变量;  
 *                   #{systemProperties['application.home']} -- Java启动时的属性(-D参数)
 *          查询集合成员： 
 *             .?[条件] -- 查询满足条件的所有元素，如 <property name="bigCities" value="#{cities.?[population gt  1000000]}" /> -- 选择所有人口数大于100万的城市
 *             .^[条件] -- 查询第一个匹配项;  
 *             .$[条件] -- 查询最后一个匹配项； 
 *             .![属性] -- 集合投影(从集合的每一个成员中选择特定的属性放入新的集合中)，如 <property name="cityNames" value="#{cities.![name]}" /> -- 获取所有城市的名字并放入新的集合中 
*******************************************************************************************************************************************************************/

public class SpELTest extends TestCase {

	@Test
	public void testSpEL(){
		assertEquals(1, 1);
	}
}
