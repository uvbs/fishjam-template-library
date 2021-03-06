"use strict";

/******************************************************************************************************************************************
* TODO:
*   1.如果选中，则移出selected类，否则就加上selected类(这是什么语法?) -- $(this)[hasSelected ? "removeClass" : "addClass"]('selected')
*     等价于: if(hasSelected){ $(this).removeClass('selected');} else { $(this).addClass('selected'); }
*     $(this)["removeClass"]('selected') 等价于 $(this).removeClass('selected');
*   2.jQuery做动画效果时要求在标准模式下，否则可能引起动画抖动(即  -//W3C//DTD XHTML 1.0 Transitional )
*   3.有的时候选择器为什么要加上 ",this" ? 比如 《锋利的jQuery中》P152,5.1.4下拉框应用中， $var options=$("option:selected",this); //获取选中的选项
*   5.jsonp 非官方协议用于实现跨域访问(从服务器端直接返回可执行的JavaScript函数调用或JavaScript对象)，已成为各大公司的Web应用程序跨域首选
*   6.很多jQuery的函数都是可链接(Chainable)的，表示支持以下语法 $("#obj").fun1().fun2();
*
* 可以在线测试各种数据的地址(照片分享网站) -- TODO: 淘宝的链接转换?
*   $(.getJSON("http://api.flickr.com/services/feeds/photos_public.gne?tags=car&tagmode=any&format=json&jsoncallback=?",function(data){ xxx });
*   使用 jsonp方式。jQuery 将自动把URL里的回调函数，如 "url?callback=?" 中的后一个"?"替换为正确的函数名，以执行回调函数。
* 
* 常见jQuery的写法
*   下拉列表(select)默认选中第一项: Xxx.find("option:eq(0)").attr("selected","selected"); 找到选中项: .find("option:checked")
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* 08第八章\第八章节例子
*
* 开发IDE(代码自动提示)
*   1.Dreamweaver + jQuery_API.mxp 插件(Dreamweaver cs6 已经内置)
*   2.Aptana -- 基于Eclipse，专注于JavaScript的Ajax开发IDE，(Windows -> Preferences -> Aptana ->Editors -> Javascript -> Code Assist)
*       支持中文: Windows -> Preferences -> General -> Content Types -> Text 默认编码为 UTF-8, File associations 下添加文件类型，如 *.js!
*   3.Eclipse + jQueryWTP 或 Spket 插件
*   4.Visual Studio 2008 
*     + KB958502 补丁(http://archive.msdn.microsoft.com/KB958502)
*     + jquery-vsdoc.js(http://ajax.aspnetcdn.com/ajax/jQuery/jquery-1.9.1-vsdoc.js, 该文件版本和jQuery一致，而且 "-" 前面的部分必须和jQuery文件名一样，放在相同目录)
*     TODO: 实测似乎不行? 但 VS2010的ProjectTemplatesCache\...\WebApplication20.zip 等目录下有 jquery-1.4.1-vsdoc.js 文件
* 辅助工具:
*   1.Chrome中有一款jQuery的扩展程序，  
* 
* JQuery(jquery.com) 免费、开源(使用MIT许可协议)，是一个轻量级的JavaScript库(精简后只有20多K)，兼容各种浏览器,极大地简化了 JavaScript 编程
*   能方便地处理HTML元素、事件等，实现动画效果，为网站提供AJAX交互，使得HTML页保持代码和页面分离(不用插入js进行调用，只需定义id即可)
*   
* 使用：head中包含 <script type="text/javascript" src="jquery.js"></script>
*       jquery.js 可以保存到本地或者服务器直接引用，也可以直接使用CDN提供的(优点:用户已经访问过时会缓存;缺点:万一连不上?)
*       Google CDN: http://ajax.googleapis.com/ajax/libs/jquery/1.8.0/jquery.min.js  -- 注：1.8.0 可以省略成 1.8 或 1 等(自动选择满足要求的最高版本)
*   两个版本：Production version -- 已被精简和压缩，用于实际的网站
*              Development version -- 完整无压缩，用于测试、学习和开发
*   文件名:
*     jquery-x.y.z-vsdoc.js -- 包含完整提示注释的版本,在VS里编写代码时会有智能地显示提示信息及注释?
*     jquery-x.y.z.js -- 标准版本
*     jquery-x.y.z.min.js -- 压缩精简后的版本，为的是在客户端减少下载时间.
*
* jQuery对象 和 DOM 对象对比
*   jQuery对象 -- 通过jQuery包装DOM对象后产生的对象(是一个数组)，只能使用jQuery的方法。命名时通常在变量名前面加上 "$"
*   DOM 对象 -- 通过 document.getXxx 返回的对象
*      $("#foo").html() <== 等价于 ==> document.getElementById("foo").innerHTML
*      jQuery变量转换为DOM: var domObj = $jObj[index] 或 $jObj.get(index), 本质是从数组里取元素
*      DOM变量转换为jQuery: var $jObj = $(domObj), 本质是通过 $(objName) 工厂函数生成, 如 $(document), $(this) 等
*
* 基础语法： $就是 jQuery的一个简写形式，如 $("#foo") 和 jQuery("#foo") 等价; $.ajax 和 jQuery.ajax 等价
*   $(selector).action() -- 美元符号定义 jQuery; 选择符(selector)查询和查找HTML 元素; action() 执行对元素的操作
*   $(document).ready(function(){ xxx }); -- 等待dom元素加载完毕执行指定语句，可以简写为 $(function(){ xxxx });
*  
* 事件: -- 如事件处理函数中 return false 则会同时 停止事件冒泡 和 阻止默认行为
*   事件对象(event) -- 事件处理函数中才能访问，事件处理函数执行完毕后，该对象就被销毁
*     metaKey/ctrlKey -- 键盘事件中获取 <ctrl> 按键 -- 两个有什么区别(jQuery通过 metaKey 统一了各个环境的<ctrl>键?)
*     originalEvent -- 指向原始的事件对象
*     pageX/pageY -- 获取到光标相对于页面的 x 坐标和 y 坐标(封装各种浏览器的区别 -- 名字和滚动条等 )
*     relatedTarget -- 事件相关元素(如 mouseover 事件的 IE::fromElement, mouseout 事件的 IE::toElement )
*     target -- 获取到触发事件的元素， 如 button, a 等，都是object，可以进一步获取具体的属性值
*     type -- 获取到事件类型的属性，如 "click" 等
*     which -- 在鼠标单击事件中获取到鼠标的 左(1)、中(2)、右(3)键，在键盘事件中获取键盘的按键(不区分大小写，如 a/A 都是 65)
*     stopPropagation() -- 停止事件冒泡函数
*     preventDefault() -- 阻止默认行为函数(如 单击超链接的跳转，单击提交按钮后提交表单 等)
*   事件冒泡 -- 嵌套元素的事件依次从内向外激发, 比如 button.onclick() -> body.onclick 等 ( 该机制可能会引起预料之外的效果 ?) 
*   事件捕获 -- 从最外层元素开始，依次到最里层元素(注意：因为不是所有浏览器都支持事件捕获，且jQuery无法修复，因此jQuery不支持事件捕获)
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* 选择器(Selectors) -- (基本语法同CSS选择器),允许开发者使用从CSS1到CSS3几乎所有的选择器及jQuery独创的高级而复杂的选择器，通过插件还可以支持XPath选择器，甚至可以编写属于自己的选择器
*   如果没有满足条件的元素(如没有指定id的元素)，调用jQuery方法也不会报错(因为选择器的返回是一个数组，只是此时数组个数为 0)
*   选择条件中有特殊字符(如 ".", "#","(",")","[","]" 等)的，需要在特殊字符前使用双斜线(\\)进行转义，如 id="myId.index" 对象的选择器: $("myId\\.index")， 但最好不要使用特殊字符
*   querySelectorAll() -- W3C在SelectorsAPI草案中提到的标准方法，为浏览器(IE8以后)原生，jQuery可能会使用该方法来重构选择器
* 
*   基本选择器
*     tagType -- 选择所有指定类型的元素， 如类型 <p> 的 $("p"), $("input[type='button']")选择所有button按钮(等价于 :button ?)
*     #IdObj -- 选择  id="IdObj" 的元素，可以选取任何元素
*       如对于 <link type="text/css" id="cssfile" href="css/skin_0.css"/>, 通过 $("#cssfile").attr("href","css/"+this.id+".css") 可切换使用的css文件，即动态换肤功能
*     .className -- 选择所有 class="className" 的元素
*     this -- 选择当前元素,通常用在事件处理代码中，表示激发该事件的对象
*     sel1,sel2 -- 将多个选择器匹配的元素合并后一起返回
*     * -- 匹配所有元素 
*   层次选择器
*     ancestor descendant -- 选取ancestor元素里的所有descendant元素(包括所有儿孙), 如 $("div span")选取<div>里的所有<span>，注意选择的是 <span>
*     parent>child -- 选取 parent 元素下的 child 子元素(只包括儿子),如 $("div > span")选取<div>元素下元素名是<span>的子元素
*     prevSel+nextSel -- 选取紧接在 prevSel 元素后的 nextSel 元素，更常见的用法是 $("prevSel").next("nextSel")
*     prevSel~siblings -- 选取 prevSel 元素之后的所有 siblings 元素，更常见的用法是 $("prevSel").nextAll("siblings)
*  过滤选择器 -- 通过特定的过滤规则来筛选
*     :first -- 选取第一个元素，如 $("div:first")选取第一个<div>元素
*     :last -- 选取最后一个元素
*     :not(selector) -- 去除所有与给定选择器匹配的元素，如 $("input:not(.myClass)") 选取class不是myClass的<input>元素
*     :even -- 选取索引是偶数的所有元素，索引从0开始，常用于表格的交替颜色
*     :odd -- 选取索引是奇数的所有元素，索引从0开始，常用于表格的交替颜色
*     :eq(index) -- 选取选取索引是index的单个元素，索引从0开始，而且只匹配一个元素
*     :gt(index) -- 选取索引大于index的所有元素(大于 index 而不包含index)
*     :lt(index) -- 选取索引小于index的所有元素(小于 index 而不包含index)
*     :header -- 选取所有的标题元素(h1~h6)
*     :animated -- 选取当前正在执行动画的所有元素
*   内容过滤选择器 -- 根据包含的子元素或文本内容进行选择
*     :contains(text) -- 选取自身或子元素中含有文本内容为"text"的元素， 如 $("div:contains('我')") 选取含有文本"我"的<div>元素
*     :empty -- 选取不包含子元素或文本的空元素, 和 :parent 相反？
*     :has(selector) -- 选取含有选择器所匹配的元素的元素，如 $("div:has(p)") 选取含有 <p>元素的<div>元素，注意选择的是 <div>
*     :parent -- 选取含有子元素或者文本的元素(即自身是父控件)，如 $("div:parent") 选取拥有子元素(包括文本元素)的<div>元素
*   可见性过滤选择器
*     :hidden -- 选取所有不可见的元素，包括 <input type="hidden"/>, style="display:none", style="visibility:hidden;" 等的元素
*     :visible -- 选取所有可见的元素
*   属性过滤选择器
*     [attribute] -- 选取拥有此属性的元素， 如 $("div[id]") 选取拥有id属性的<div>元素
*     [attribute=value] -- 选取属性值为 value 的元素，如 $("div[title=test]") 选取属性title为"test"的<div>元素
*        对应的有 !=value(不等于), ^=value(属性值以value开始), $=value(属性值以value结束), *=value(属性值含有value)
*     [selector1][selector2] -- 将属性按“与”的方式合并，同时满足多个条件，如 $("div[id][title*='test']") 选取有属性id，并且属性title中含有"test"值的<div>元素
*   子元素过滤选择器(注意和过滤选择器的区别：1.子元素过滤选择器的索引从1开始; 2.子元素过滤选择器会对每个父元素都过滤)
*     :nth-child(index/even/odd/equation) -- 选取每个父元素下的第index个子元素/奇/偶/?? 元素(index从1开始算)，如 $("div.one :nth-child(3n)")选取class为one的div父元素中索引值是3的倍数的元素
*        注意：为每一个父元素匹配子元素，并且index是从1开始的
*     :first-child -- 选取每个父元素的第一个子元素，如 $("ul li:first-child") 选取每个<ul>中第一个<li>元素
*     :last-child -- 选取每个父元素的最后一个子元素
*     :only-child -- 如果某个元素是它父元素中唯一的子元素，将会匹配(即父元素中只有一个子元素)，如 $("ul li:only-child") 在<ul>中选取是唯一子元素的<li>元素
*   表单对象属性过滤器 -- 对所选择的表单元素进行过滤
*     :enabled/:disabled -- 选取所有 可用/不可用 元素
*     :checked -- 选取所有被选中的元素(单选框/复选框)
*     :selected -- 选取所有被选中的选项元素(下拉列表), 如 $('#mySelect option:selected')
*   表单选择器 -- 选取表单(form)内制定类型的元素,使用以前通过指定form或其他属性限制选择范围, 如 $("form1 [name=items]:checkbox") 选择form1中name属性为"items"的所有checkbox
*     :button -- 选取所有的按钮
*     :checkbox -- 复选按钮
*     :file -- 
*     :hidden -- 所有的不可见元素，<input type="hidden" />和<div style="display:none">test</div>都可以匹配.
*     :input -- 选取form中所有的输入类元素(input/textarea/select/button等)，注意和 $("input")的区别--":input"比"input"要多
*     :image -- 
*     :password 
*     :radio -- 单选按钮
*     :reset -- 重置按钮
*     :submit -- 提交按钮
*     :text -- 选取所有的单行文本框
*     textarea -- 前面没有 冒号(:),采用的是 tagName 方式
*  选择器例子：
*    1.组合多个选择器来选择: <input type="checkbox" name="items" xxx> 中选中项 -- $('[name=items]:checkbox:checked')
******************************************************************************************************************************************/

/******************************************************************************************************************************************
*
* 插件(模块化的使用方式)：
* DOM操作封装 -- 封装了大量常用的DOM操作
* 可靠的事件处理机制 -- 
* 完善的Ajax -- 将所有的Ajax操作封装到一个函数 $.ajax() 里，开发者处理Ajax时能专心处理业务逻辑
* 不污染顶级变量 -- 只建立一个名为jQuery的对象，其所有的函数方法都在这个对象之下，其别名 $ 也可以随时交出控制权( jQuery.noConflict();然后可以 jQuery(xxx) 的方式使用 )，
*   绝不会污染其他的对象(可和其他库共存而无冲突)
* 出色的浏览器兼容 -- 
* 链式操作方式 -- 对发生在同一个jQuery对象上的一组动作，可以直接连写而无需重复获取对象(代码优雅)
* 隐式迭代 -- 方法都被设计成自动操作对象集合，而不是单独的对象，不需要编写大量的循环结构代码
* 行为层与结构层的分离 -- 可以使用jQuery选择器选中元素，然后直接给元素添加事件，这种分离的是想可以是jQuery开发人员和页面开发人员各司其职
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* $(html [,context]) -- 工厂函数,如果没有指定context参数，将在当前的HTML文档中查找DOM元素,如果指定了context，那就会在这个context中查找。
*    如果内容是已有的dom变量，则转换成jQuery变量[ 如 $(document) ]; 
*    否则根据html标记字符串(注意需要是闭合的XHTML格式)创建DOM对象，并封装成jQuery对象返回[ 如 $parent.append($("<li>新列表项</li>")) ]; 
*    如果在 函数里直接写 "xxx" 等价于 $("xxxx")?
*
* 实用全局函数和属性
*   $.support -- 返回浏览器对各种特性的支持键值对，如 $.support.ajax 判断是否支持 Ajax
*   
* 函数(TODO:分类别来写) -- 注意：jQuery中很多功能使用相同的名字实现 getter/setter
*   控制元素 -- 注意：如果操作的元素是已有的，则通常是移动操作，如 $parent.append("$obj") 时$obj是已有的元素，则会从原来的位置移动到 $parent下
*     .after($siblingObj)/.before($siblingObj) -- 在每个匹配的元素之 “后/前”  插入内容，是其兄弟元素
*     .append($obj) -- 追加子元素到父元素的最后面?(在 innerHTML 属性中增加?)，是其子元素
*     .appendTo($parent) -- 将匹配元素追加到指定父元素的最后面
*     .clone([bool]) -- 复制匹配节点并返回，如果参数为true表明其副本具有同样的绑定事件
*     .empty() -- 清空此元素里的内容(即其内容和所有后代节点)，但元素还存在，如 $("ul li:first").empty() 会清除其文本，但节点(<li>)还在
*     .insertAfter($siblingObj)/.insertBefore($siblingObj)  -- 插入到指定的兄弟节点 “后/前” 面
*     .prepend($obj) -- 插入到父元素的最前面
*     .prependTo($parent) -- 将匹配元素插入到指定父元素的最前面
*     .remove() -- 从DOM中删除所有匹配元素及后代节点，返回被删除元素的引用
*     .remove(Selector) -- 从本对象中移除指定条件的子对象
*     .replaceWith($obj) -- 使用指定元素替换匹配元素
*     .replaceAll(Selector) --使用匹配元素替换所有指定元素?
*   筛选节点 (注意：各个函数都可以使用jQuery表达式作为参数来筛选元素)
*     .closest() -- 取得最近的匹配元素，首先检查当前元素是否匹配，如匹配直接返回元素本身，如不匹配则逐级向上查找父元素，直到找到匹配的或空jQuery对象
*       如 给点击的目标最近的 li 元素添加颜色: $(document).bind("click", function(e){ $(e.target).closest("li").css("color", "red"); })
*     .children() -- 取得匹配元素的子元素集合(不考虑后代元素)
*     .contents() -- 查找匹配元素内部所有的子孙节点(包括文本节点)
*     .eq(index) -- 从匹配元素集中取得指定位置的元素，index从0开始
*     .filter() -- 按照指定条件过滤，如 input.keyup 中根据用户的输入进行过滤显示 .filter(":contains('"+( $(this).val() )+"')").show();
*     .find() -- 在匹配元素中查找指定(子？)元素,如 .find("tr")
*     .is(expr) -- 用一个表达式来检查当前选择的元素集合，如至少有一个元素符合给定的表达式就返回true
*       .is(".className") -- 判断是否含有某个式样类
*       .is(":属性") -- 如 :checked(是否被选中), :visible(是否可见), :animated(是否处于动画)
*       .is("#id") -- 判断是否是指定元素
*     .map(callback) -- 将jQuery对象中的一组元素利用callback方法转换其值，然后添加到一个jQuery数组中
*     .next()/.prev() -- 取得匹配元素 后面/前面 紧邻的同辈元素
*     .parent() -- ? 取得一个包含所有匹配元素的唯一父元素的元素集合(即得到直接父元素?)
*     .parents() -- ? 递归在各级父元素中查找指定元素(祖先元素)
*     .siblings() -- 取得匹配元素前后满足条件的所有同辈元素(不包括自身)
*     其他的: .nextAll, .prevAll,
*   属性样式控制
*     .attr("属性" [,"新值"]) -- 返回或设置指定的属性值, 如 class/style/title/checked 等, 如要一次性设置多个属性，可以使用如下格式：
*        .attr({ "title" : "your title", "name" : "your name" });
*     .addClass("className") -- 增加指定的class属性(通常用于关联CSS)，注意：采用追加方式，有多个时会成为 "class1 class2" 等多个值的合并(如有相同名字的属性，后加的覆盖先加的)
*     .css("属性" [,"新的值"]) -- 读写对应的css属性值, 如 .css("font-color", "red") 
*        注意：无论对应的属性是外部CSS导入、还是内联，或动态设置的？都能获取到最终的结果
*        常见属性(部分和标准CSS不一样): backgroundColor, color, fontSize/font-size(文字大小), opacity(透明度0~1)
*     .data(name [,value]) -- 读写元素上存储的响应名字的数据
*     .hasClass("className") -- 判断是否有指定的class属性,增强代码可读性而产生的,等价于 .is(".className")
*     .height([xx])/width([x]) -- 读写以px为单位的实际高度和宽度(如果用 .css("height") 得到的可能是"auto"等)
*     .innerHeight/innerWidth -- 内部高度/宽度(不包括border，但包括padding)
*     .offset() -- 读写在当前视窗的相对偏移，返回的对象包含 top 和 left 两个属性
*     .outerHeight/outerWidth -- 外部高度/宽度(包括border和padding)
*     .position() -- 获取元素相对于最近的一个position样式属性设置为 relative 或者 absolute 的祖父节点的相对偏移，返回的对象包含 top 和 left 两个属性
*     .removeAttr(["属性"]) -- 移除特定的属性，如 removeAttr("style")表示清除其所有的CSS属性, 如 "属性"为空，则去除所有的属性?
*     .removeClass(["className1 className2"]) -- 去除指定的class属性，如果 className 为空，则去除所有的class属性
*     .scrollTop([value])/.scrollLeft([value]) -- 读写元素的滚动条距顶端和左端的距离。有同名属性?( $comment.animate({ scrollTop:"-=50" } , 400);)
*     .toggleClass("className") -- 重复切换指定类名(存在则删除,不存在则添加)
*     .wrap("<a href='http://www.baidu.com' target='_blank'></a>") -- 使用指定元素把匹配元素包裹起来
*     .wrapAll("<xxx></xxx>") -- 将匹配的所有元素放在一个 <xxx> 中, 而 .wrap 会对每个匹配的元素都包裹一个 <xxx>
*     .wrapInner("<xxx></xxx>") -- 将每一个匹配的元素的子内容(包括文本节点)用其他结构化的标记包裹起来，即 包裹在内部
*   文本函数
*     .html(["新的值"]) -- 读取或设置其 HTML 值，等价于domObj.innerHTML, 注：不能用于XML文档
*     .text(["新的值"]) -- 读写元素中的文本内容,等价于 domObj.innerText, 如 <p>中的文本，select中每一个option的文本
*     .val(["新的值"]) -- 读写元素的值,类似domObj.value(用于 button,select,text等的文字显示)
*        如果需要设置多选值(如 multiple 的 select)时，需要用中括号括起且逗号分隔的项(如: $("#multiple").val(["check2","check3"]); )
*        对应的值是 <option>中的text部分, checkbox,radio 的 value或text部分
*   动画函数 -- 为了避免动画堆积，在增加动画代码前需要先判断是否处于动画： if(!$Obj.is(":animated")){ 不处于动画，可以增加 }
*     .animate( {属性名1:"属性值1"[, 属性名2:"属性值2"] } [, 时间] [, 结束时的回调函数] ) -- 自定义的动画函数，在指定时间段内更改属性值达到动画效果，
*        如 $(this).animate( {left:"500px", height:"+=200px" }, 3000);
*        具有最强大的功能，可以模拟出其他动画，如 .show(600) == .animate( {height:"show", width:"show", opacity:"show" } , 600 );
*     .fadeIn/.fadeOut -- 通过更改 "透明度" 来 "淡入/淡出" 元素
*     .fadeTo(时间, 透明度, 回调) -- 更改透明度
*     .show([时间])/.hide([时间]) -- 同时更改 "高/宽/透明度" 来 "显示/隐藏"匹配元素, hide前会先记住原先的 display 属性值，用于show时恢复
*     .slideDown/.slideUp -- 通过更改 "高度" 来 "显示/隐藏" 元素
*     .slideToggle( 时间, 回调) -- 通过更改高度来动画切换可见状态
*     .stop([clearQueue=false] [,gotoEnd=false]) -- 停止匹配元素的动画，clearQueue为true表示清空未执行完的动画队列， gotoEnd为true表示直接将正在执行的动画跳转到末状态。
*     注意：
*       1.时间可以用 "slow"(600ms)/"normal"(400ms)/"fast"(200ms) 或 毫秒数 来指定
*       2.多个动画函数可以链式操作形成动画队列，依次执行
*       3.stop时只能设置正在执行的动画最终状态，而没有提供直接到达未执行动画队列最终状态的方法
*       4.TODO:使用动画函数来制作动画性能不高 -- HTML5中使用Canvas绘制更好?
*   串联函数
*     .andSelf() -- 将前一个匹配的元素集合添加到当前的集合中
*     .end() -- 结束最近的“破坏性”操作，将匹配的元素集合恢复到前一个状态，即重新定位到上次操作的元素?
*
*   数组和对象函数
*     .each(function (){ xxx }) -- 对选择出来的每一个元素执行指定事件
*     .grep(array, callback, invert) -- 使用过滤函数过滤数组内容
*     .inArray(value, array) -- 返回value在数组中的位置，如果没有找到，则返回 -1
*     .isArray(obj) -- 判断是否是数组
*     .isFunction(obj) -- 判断是否为函数
*     .makeArray(obj) -- 将一个类似数组的对象转化成数组
*     .merge(first, second) -- 合并两个数组并返回
*     .unique(array) -- 删除元素数组中的重复元素(不能用于普通数组?)
*   其他函数
*     ?.children(selector) -- 通过指定的选择器选择所有满足条件的子元素?
*     .expr() -- ?
*     .filter(Selector) -- 根据指定条件过滤出满足需求的子元素集合, 如 $obj.filter(":contains('佳能'),:contains('尼康')")
*     .get(index) 或 [index] -- 选取jQuery对象数组中指定的元素，注意：返回值是DOM对象
*     .index(domObj) -- 获取指定 dom 对象在匹配元素中的索引值整数，如 var index= $objs.index(this) -- 获取当前激发事件的元素在全部匹配元素中的索引
*     .next("xx")
*     .nextAll("xxx") -- 选取匹配元素 后面的 同辈"xxx"元素
*     .serialize() -- 表单对象序列化，通常作用于 表单form 或 input元素 上，可以将用户输入序列化成 GET 的参数字符串(会自动调用 encodeURIComponent()编码 )
*        如: $.get("get1.asp", $("#form1").serialize(), function ... );
*     .serializeArray() -- 表单对象序列化成JSON格式的数据并返回，然后可以通过 $.each 进行迭代处理
*     .siblings("xxx") -- 选取匹配元素同辈的"xxx"元素
*     .toggle([时间] [,回调]) -- 切换元素的可见状态，注意和 toggle合成事件 的区别
*     .trigger("eventName", [ 参数数组 ] ) -- 模拟激发指定事件，如 $("#isreset").trigger('click')激发 onclick事件, 也可用 .click()代替，并继续执行浏览器的缺省操作
*        注意：可以激发自定义的事件(自定义事件也只能这样激发？)
*     .triggerHandler("eventName") -- 模拟激发指定事件(但不继续执行浏览器的缺省操作，如激发 $inputObj.triggerHandler("focus") 会激发事件，
*        但不会将焦点设置到 $inputObj 上)；或者 在 .blur 里面验证输入时，可以在.focus和.keyup 中triggerHandler("blur")来激发实时验证
*     $.extend(target, {新键值对} ) -- 扩展target的值，如果新的键值对有新的值会用新的，否则用target里的旧键值对，通常可用在编写插件时设置缺省值
*       如: function foo(options) { options = jQuery.extend({name:"test", length: 5 }, options); //前面是默认参数， options 是传递的参数
* 
*   Ajax封装函数(以 $. 开始的是全局函数， 以 $obj. 开始是指定对象上的函数 )
*     $.ajax( { 各个参数的键值对 } ) -- 底层AJAX实现的全局函数，比如  $.ajax({ type:"POST", url:"delUser.do", data:"id="+id, dataType:"json" }); 
*       等价于 .getScript("test.js")
*         属性
*          async -- 是否是异步请求，默认为true
*          cache -- 是否缓存，默认为true， 但type为POST或dataType为script时默认为false
*          contentType -- 当发送信息至服务器时的内容编码类型，默认为"application/x-www-form-urlencoded"
*          data -- 发送到服务器的键值对数据或数组、字符串等，如：$("#form1").serialize() 或 a=1&b=2&c=3
*          dataType -- 预期服务器返回的数据类型，如 html/json/jsonp/script/xml/text 等，若不指定则自动根据HTTP包的MIME信息返回xml或text等
*          global -- 布尔值，表示是否触发全局Ajax事件( $.ajaxStart/$.ajaxStop 等)，默认为true。
*          jsonp -- 在一个jsonp请求中重写回调函数的名字，用来替代在 "callback=?" 这种请求中的"callback"部分。如 {jsonp:'onJsonPLoad'}会传递"onJsonPLoad=?"给服务器
*          processData -- 控制是否将发送的数据转换为对象以配合默认的 contentType("application/x-www-form-urlencoded"),默认为true
*          timeout -- 超时时间(毫秒),将覆盖 $.ajaxSetup 方法的全局设置
*          type -- GET/POST 等，在远程请求时(不在同一个域下)，所有对 script 的POST请求都会转为GET请求
*          url  -- 目标URL,如 "test.asp" 等
*          username/password -- 用于响应HTTP访问认证请求的用户名和密码
*        事件回调 -- TODO:回调函数中的this是指本次请求时传递的options参数？
*          beforeSend -- 提交前的回调函数,如返回false会取消本次请求， function(xmlHttpRequest){ }
*          complete -- 不论请求是否成功，只要请求完成都会调用的回调函数,function(xmlHttpRequest, textStatus){ }
*          dataFilter -- 返回的原始数据预处理函数， function(data, dataType) { return 过滤过的data; }
*          error -- 请求失败时的回调函数, function(xmlHttpRequest, textStatus, errorThrown){ }，
*          success -- 请求成功后的回调函数, function(data, textStatus){xxx}，注意：回调函数的参数可能根据 dataType 不同而不同(见 .getJSON 和 .getScript 的回调函数)
*     $.ajaxSetup(options) -- 设置全局Ajax选项
*     $.each(数组, function(index, 内容) { 处理逻辑 }) -- 遍历数组(如 匹配结果、XML、JSON元素)进行处理
*     $.get("URL", { 参数键值对 }, 成功时的回调函数 ,返回类型 ) -- 使用GET的全局获取函数，可通过在URL后 "+(+new Data)" 的方式来禁用缓存。
*          注：（+new Date) 等价于 new Date().getTime() 
*        参数键值对 -- 键值不需要使用引号，如 username : $("#username").val()，会采用 ?name1=value1&name2=value2 的方式提交给服务器
*        回调函数格式 -- function (data, statusText) { xxx }, 
*        返回类型 -- 可选，如 "_default","html","json","script","text","xml" 等，如不写则是字符串。
*          回调中返回的data会根据该类型分别是 XML对象字符串，JSON解析对象，字符串等
*     $.getJSON(".json地址" [, function(data){ 成功加载完毕的回调函数 }]) -- data就是已经解析出的JSON对象？
*     $.getScript(".js地址" [, function(){ 成功加载完毕后的回调函数 }]) -- 全局函数，动态加载并执行 JavsScript 文件
*     $.param("键值对参数") -- 将键值对参数序列化成字符串格式， 如 $.param({a:1,b:2,c:3}) 返回 "a=1&b=2&c=3"
*     $.parseJSON("json格式的字符串") -- 将JSON字符串解析成JS对象
*     $.post("URL",{ 参数键值对 }, 成功时的回调函数, 返回类型 ) -- 全局POST的全局提交函数，基本同 $.get，但post默认时会禁用缓存。
*        回调函数格式 -- function (data, statusText) { xxx } 
*     $obj.load("URL[ selector]" [,{参数键值对}] [,回调函数] ) -- 在匹配元素上加载指定URL对应的内容，没有参数时采用GET方式，有参数时使用POST方式
*        GET方式 -- 直接使用 xxx.asp?username=xxx&content=yyy 等的URL形式
*        POST方式 -- 通过 参数键值对提供
*        URL参数可以通过选择符来记载指定的元素，比如 $("#resText").load("test.html .para"); 只加载 test.html 中class为".para"的内容
*        回调函数的格式 -- function (responseText, textStatus, XMLHttpRequest){ xxx }
*  
* 事件( .xxx(function([event]) { xxx });
*     .ajaxStart()/.ajaxStop() -- AJAX开始和结束时的事件，注意：必须写成类似 $("#loading").ajaxStart 的方式
*       .ajaxComplete(请求完成，无论成功失败?), .ajaxError(请求发生错误),.ajaxSend(发送前), .ajaxSuccess(请求成功时)
*     .bind("事件1 [事件2]" [,data] , fun) -- 通用地绑定指定事件, data是可选参数，作为 event.data 属性值传递给事件对象的额外数据对象
*     .blur -- 失去焦点时
*     .change() -- 设置change事件，如 $("select").change(function(){ xxx; })
*        TODO: <select> 用 .live("change", function(){...});
*     .click -- 设置click事件
*     .error -- 
*     .focus -- 获得焦点时
*     .keyXxx -- 键盘事件: .keydown/.keypress/.keyup
*     .live(type, fn) -- jQuery1.3.1中新增了 .live(type,fn) 方法动态绑定事件(类似livequery，但功能没那么强?),取消绑定时使用 die(type,fn)
*        TODO: 和.bind的区别?
*     .load -- 等价于 onload
*     .mouseXxxx -- 鼠标事件: .mousedown/.mouseup/.mousemove(内部移动时)
*                             .mouseover/.mouseout -- 不论鼠标指针穿过被选元素或其子元素，都会触发 mouseover 事件
*                             .mouseenter/.mouseleave -- 只有在鼠标指针穿过被选元素时，才会触发 mouseenter 事件(子元素不触发)
*     .one("事件" [,data] , fun) -- 通用地绑定一次事件(只激发一次)
*     .ready() -- 如 $(document).ready(function(){ xxx }); 
*        网页中所有DOM完全就绪就执行，但此时DOM元素关联的东西可能没有加载完(比如图片)，相比 window.onload 能极大提高响应速度(onload是所有元素加载完才执行),
*          可以同时编写多个(但window.onload只能有一个)
*        缺点：ready中可能无法准确获取图片的信息(如高宽等)，对应的有 load 事件；
*      .resize --
*      .scroll, .unload, .dblclick, .select, .submit
*      .unbind(["事件" [,fun] ] ) -- 取消事件的绑定，可以指定 取消哪个事件或事件上的具体函数
*   自定义事件 -- $obj.bind("myEvent", function(event, param1, ...paramN){ xxxx }
*     激发(必须用 trigger，且通过数组的形式传递参数)：$obj.trigger("myEvent",["参数1","参数2"]);
*   事件的名称空间 --  对事件可以指定名称空间(如 click.fishjam ), 然后可以根据名称空间来操作(如 unbind(".fishjam") 可取消 ".fishjam" 名称空间下的所有事件,
*     激发时可在 trigger 的事件名字后加感叹号("!") 来匹配所有不包含在命名空间中的方法，如 "click" 会激发所有空间下的, "click!"只激发默认空间下的
*   合成事件(jQuery中自定义的方法)
*     .hover( enter, leave ) -- 模拟光标悬停事件，分别对应 mouseenter 和 mouseleave 事件
*     .toggle( fun1,fun2,...funN ) -- 模拟鼠标连续单击事件，可以在每次单击时切换多个函数的处理逻辑
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* 丰富的插件支持(http://plugins.jquery.com/) -- 
*   Basic XPath -- 可以让用户使用基本的XPath
*   Color -- jquery.color.js，颜色动画插件
*   Cookie -- jquery.cookie.js，方便cookie操作， $.cookie("名字" ,新值 ,{ 附加属性的键值对，如 path: '/', expires: 10 });
*     删除 -- $.cookie("名字", null, { path: '/' });
*     expires -- 10天 -- 10; 三天后 -- var date = new Date(); date.getTime() + (3 * 24 * 60 * 60 * 1000); { xxxx, expires: date }
*       如果不设置或设置为null，会作为 Session Cookie处理(浏览器关闭后删除)
*   Form -- jquery.form.js，表单插件，可以无侵入地升级HTML表单以支持Ajax。提供 .ajaxForm/.ajaxSubmit/.formSerialize 等方法
*     ajaxForm(); -- 
*     ajaxSubmit({url:'/xxx.do', dataType:'json', success:function (data, statusText){成功时执行的代码}, error:function(){失败时执行的代码} *   jQueryUI -- UI扩展库，主要分为三个部分
*     交互(ui.sortable.js 等) -- 与鼠标交互相关的内容，可以很容易地支持 拖拽(.sortable({delay:1}))、缩放、选择、排序等;
*     Widget(ui.core.js 等) -- 一些界面的扩展，如 自动完成(Autocomplete),取色器(Colorpicker),对话框(Dialog),滑块(Slider),放大镜(Magnifier) 等;
*     效果库(effects.core.js) -- 提供丰富的动画效果，不再局限于 animate 方法,
*   jQZoom -- jquery.jqzoom.js, 放大缩小图片(可用于商品的放大镜效果)
*   LiveQuery -- jquery.livequery.js，可以使用统一的语法绑定 前期静态生成的元素 和 后期通过脚本动态生成的元素，如  $('a').livequery('click', function(){ xxx });
*   Metadata -- jquery.metadata.js，支持固定格式解析，可用于Validation中编写验证规则
*   MoreSelectors for jQuery -- 增加更多的选择器，如 .color 匹配颜色, :colIndex 匹配表格中的列, :focus 匹配获取焦点的元素等
*   Validation -- jquery.validate.js，表单验证插件(内置多种验证规则：必输入项、邮件、URL、长度等)，可以自定义规则、提示信息，支持多语言等。
*     加载.js文件后，可在 .ready 中通过 $("#myForm").validate( { JSON 语法的规则、提示信息、回调函数 等 } ) 初始化需要验证的信息，
*     可通过 $.validator.addMethod( "验证方法名", function(value, element, param) { 验证规则 }, "错误时提示信息");
* 
* 编写插件 -- 给已有的一系列方法或函数做一个封装，以便在其他地方重复使用。插件文件名推荐为 "jquery.xxx.js"
*   主要有三种类型：
*     1.封装对象方法的插件，常用语对通过选择器获取的jQuery对象进行操作(如 .parent,.color 等)
*     2.封装全局函数的插件(jQuery中的全局函数)，如 解决冲突的 jQuery.noConflict(), jQuery.ajax, jQuery.trim 等
*     3.选择器插件
*   要点：
*     1.所有的对象方法都应该附加到 jQuery.fn 对象上，所有的全局函数都应该附加到 jQuery 对象本身上;
*     2.在插件内部，this 指向的是当前通过选择器获取的jQuery对象，而不像一般方法中指向的DOM元素；
*       selector 返回选择此元素的选择器; context 返回选择此元素时所在的DOM节点内容
*     3.可通过 this.each 来遍历选择符匹配的多个元素
*     4.所有的方法或函数插件，都应当以分号结尾，否则压缩的时候可能出现问题。为了稳妥，甚至可以在插件头部先加一个分号，以免其他不规范的插件影响。
*     5.通常情况下，插件应该返回一个jQuery对象(比如this),以保证插件的链式操作
*   常见的插件写法（利用闭包特性使用 $作为jQuery的别名，避免污染全局空间）：(function($){ 插件扩展代码 })(jQuery);
*     两种扩展方法：
*     1.$.fn.extend({           //扩展jQuery元素，提供对象的新的函数，通过 $obj.函数名 调用
*         "函数名1":function(options){ options = $.extend({"默认参数"},options); 实现体; return this; }, //推荐的参数方式
*         "函数名2":function(value1,valuen){ 实现体2; } 
*       });
*       -- if (value==undefined){ 读取调用 }else{ 设置调用 }, 注意通常需要返回对象(如 this) 来支持链式操作
*       $.fn.toDisable = function(){ xxx } -- 什么意思? 
*       $.fn.toRestore = function(){ xxx } -- 
*     2.$.extend({              //在jQuery命名空间内添加一个全局函数，通过 $.函数名 调用
*         "函数名1":function(){ },
*         "函数名2":function(){ }
*       });
*       除了可以用与扩展 jQuery 对象外，还有一个很强大的功能，就是用于扩展已有的Object对象: jQuery.extend(target, obj1, obj2...)
*  自定义选择器(制作选择器插件 ) -- jQuery的选择符解析器首先先使用一组正则表达式来解析选择器，然后针对解析出的每个选择符执行一个函数(选择器函数)，
*    根据该函数的返回值(true或false)来决定是否保留这个元素。
*    注意：现在的jQuery版本实现和《锋利的jQuery》一书中的实现已经不同了，可仿照 如 $.gt 实现 $.between(n,m)
*          $.extend(jQuery.expr[":"], { between : function(a,i,m) { xxxx; } });
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* 解析XML -- 解析XML文档与解析DOM一样，也可用 find, children, 等来解析和用 each 来遍历， text/attr 等来获取文本和属性。
*    $(xmlString) 生成XML解析对象。如 ajax 的以XML返回时 $(responseText), 各方法和选择器类似?
*   .find("selector") -- 查找指定的节点，使用的语法类似 选择器？ 如 find("comment content").text() 可查找 <comment><content>someValue</content></comment>
*
* 解析JSON -- ajax 的 .get()以JSON返回时就是 JSON 解析对象？ 直接使用 data.varName 即可访问？
*   
* 
******************************************************************************************************************************************/

$(document).ready(function() {
    $("button").click(function() {
        $("p").hide();
    });
});

module("JQueryTester", {
    setup: function() {
        //dump("in JQueryTester::setup\n");
    },
    teardown: function() {
        //dump("in JQueryTester::teardown\n");
    }
});

test("JQuery", function() {
    var comments =
    [
        { "username": "张三", "content": "沙发.", "score": 80 },
        { "username": "李四", "content": "板凳.", "score": 70 },
        { "username": "王五", "content": "地板.", "score": 90 }
    ];
    var totalScore = 0;
    $.each(comments, function(index, comment) {
        totalScore += comment.score;
    });
    equal(totalScore, 240, ".each 遍历");

    console.log("support information is: %o", jQuery.support);
    equal($.support.ajax, true, "检测浏览器对ajax的支持");
});

