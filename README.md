# GAMCS-Lab 玩法 

## GAMCS-Lab 是什么
GAMCS-Lab 是一个用于试验和演示GAMCS学习模型的小软件。它提供了一个基本场景和几种元素(Spirit)，其中有三种元素的行为是由
GAMCS驱动的，分别是`老鼠，猫，大象`，它们在GAMCS-Lab中被称为`Avatar`即`化身`。通过往场景中添加不同的元素以及设置不同的环境，可以观察化身们的行为。
在GAMCS的驱动下，虽然每个化身的功能都很简单，但是它们的行为却表现的很有意思。

## 基本界面和操作

GAMCS-Lab的基本界面，中间是一块带有网格的场景画布，右侧是元素列表和几个控制按钮，上方是菜单栏，下面是状态栏。

### 场景 (Scene)
首先看场景，场景是由网格构成的，网格的大小和元素的大小相同，每个元素只能被添加到某个网格里，如果是可以移动的化身元素，也是以网格为基本步长。

场景四周有四条黑线，这是场景的边界，添加元素以及移动元素都不能超过这个边界。边界的大小可以在[Options->Configure]对话框设置。

通过鼠标拖拽和滚动条可以移动场景的视窗，通过鼠标滚轮可以放大、缩小场景。

通过右侧工具栏中最下方的三个按钮可以停止/运行，以及加速和减速场景。

#### 演示场景
GAMCS-Lab 默认提供了几种绘好的场景作为演示，可以通过[Scene]->[Demo Scenes]菜单来加载。
绘制好的场景可以保存成文件，方便多次使用或分享。

### 元素 (Spirits)
v0.1.0版本的GAMCS-Lab里有7种元素，分别是：砖块(Block)、奶酪(Cheese)、刺钉(Nail)、炸弹(Bomb)、老鼠（Mouse）、猫（Cat）和大象(Elephant)。
前四个被称为`静态元素`，因为它们不能运动，后三个被称为`化身元素`或简称`化身`，它们由GAMCS驱动，继承于GAMCS中的Avatar类。

它们的作用分别是：

1. **砖块(Block)**：放在某个网格中可以阻止化身通过，起到墙的作用。
2. **奶酪(Cheese)**：一种非常可口的食物，化身们都喜欢吃，它可以用来对化身进行奖励。
3. **刺钉(Nail)**：扎人的东西，化身们碰到它会被扎痛，可以用来对化身进行惩罚。
4. **炸弹(Bomb)**：非常致命的东西，化身们碰到它会立刻被炸死，可以用来瞬间消灭化身。
5. **老鼠(Mouse)**：由GAMCS驱动的化身中的一种。除了上面说过的喜欢奶酪，害怕刺钉之外，还害怕猫，喜欢咬大象。
6. **猫(Cat)**：也是一种化身，也喜欢奶酪，害怕刺钉。另外还喜欢吃老鼠，害怕大象。
7. **大象(Elephant)**：第三种化身，同样喜欢奶酪，害怕刺钉。另外还喜欢吃猫，害怕老鼠。

其中，作为化身的三种元素具有感觉和运动功能。每个化身都**能且仅能**知道自己当前所处的位置，即`(x,y)`网格坐标。
每个化身都有有五个可以选择的动作，分别是：`向上一格、向下一格、向左一格、向右一格以及原地不动`。

所有元素创建时都有一个固定生命值，它的大小可以在设置对话框里修改。当元素被咬或者被刺时，生命值会相应的减小，
当生命值减小到0时，元素死亡，将被从场景中删除；相反，当元素吃到奶酪或者它的猎物时，生命值会相应的增加。


### 元素的基本操作
要添加某个元素到场景中，首先点击工具栏上的元素图标，然后在场景上需要添加的位置处单击添加，或者在点击图标后，按住CTRL键，移动鼠标，进行批量添加。
每个元素在场景中显示为一个填满网格的方块，颜色和它的图标颜色一致。

要移动场景上的元素，首先点击工具栏上的光标图标，然后就可以对元素进行拖拽。在元素上右键鼠标会弹出菜单，可以对每个元素进行设置。

工具栏上的第二个图标是擦除按钮，点击之后可以用来擦除场景上的元素，使用方法与添加元素相同。


## 参数解释与配置

### 配置对话框
通过菜单的[Options]->[Configure]可以打开配置对话框，这里可以对场景和元素的参数进行设置。**需要注意的是**，参数的修改仅对_新创建_的场景和元素生效，
已经存在于场景中的元素参数不会被修改。

在配置对话框里，对于场景目前可以设置场景长和宽（单位网格）。对于化身，可以设置每种化身的初始生命值，交流相关的参数，以及GAM模型的内部参数。对于静态元素，
可以设置它们的初始生命值。

其他的参数都好理解，下面着重解释一下化身的几个参数。

在GAMCS-Lab里，同一种化身之间可以进行`交流`，所谓的交流就是指一个化身个体将它大脑中的一部分知识分享个另一个个体。
这种分享受两个参数调控：

* **分享范围(Share Range)**，它表示以某个个体为中心周围多大范围内的个体会接受到它的分享。在GAMCS-Lab中，
分享的范围是一个正方形，Share Range参数表示正方形的半边长，设置小于等于0的值将关闭分享。（每次进行分享时，化身的周围会闪出一个矩形虚线框）
* **分享频率(Share Freq)**，它表示个体多长时间分享一次，这个时间是以场景的运行步子为单位的，加快场景运行速度可以加速分享。最小值为1，表示每一步都分享。

涉及GAM模型的参数主要有三个：

1. **贴现率(Discount Rate)**，如果读者了解一点经济学应该很容易明白这个概念，在GAM里它表示一个化身对未来收益的看重程度。打个比喻贴现率就
代表一个化身的眼光大小，贴现率越大，化身的眼光越远，反之眼光越近。它对化身的行为有非常重要的影响，取值范围`[0, 1)`。
2. **精度(Accuracy)**，它表示一个化身对收益值的敏感程度或颗粒度。打个比方，就像穷人和富人对金钱的敏感程度不同一样，对一个穷人而言，一角钱就是钱，
而对于富人而言，也许一百块才是钱，两者对金钱的感觉颗粒度不一样。精度这个概念控制的就是化身对收益的认识颗粒度。这个量对化身的行为同样有非常重要的影响，
默认值为`0.001`,表示化身对0.001以下的收益变化都不在乎。
3. **学习模式(Learning Mode)**, 在GAMCS，学习模式有两种：一种称为在线式（`Online Mode`），另一种称为探索式（`Explore Mode`）。
两者的差别在于，在在线式，化身在不断的学习，同时根据它已经学到的东西来行动；而在探索模式，化身虽然也是在不断的学习，但是它的行为却是随机的，没有学以致用。
在线模式的特点在于边学边用；但是它的缺点也很明显，当个体数比较少缺乏交流时，个体的行为过于依赖它过去的知识和经验，缺乏创新，容易陷入局部最优。而探索模式
则很好的解决了这个问题，化身先把整个场景都学习一遍，不过早的作决定，这样容易找到全局最优解。因此，对于个体数较少时，一般的用法是，先将化身至于探索模式，
让它探索一段时间，然后再改为在线模式，让它将探索学到的东西用起来。

### 右击菜单配置
上面说过，在配置对话框中的设置仅对新创建的元素有效，对于已经存在与场景的元素，可以通过右击弹出菜单来设置。

对于静态元素，菜单项很简单，就是标记和设置生命值，功能显而易见。对于化身元素，菜单项稍微复杂一些，下面化身特有的几项作些解释：

1. **睡眠(Sleep)/唤醒(Wake up)**，表示将化身置于睡眠/唤醒状态，处于睡眠的化身不会吃东西，也不会被其他化身咬。正常状态为唤醒。
2. **示教(Teach)/取消示教(Un-Teach)**，这是GAMCS v1.3.6 引入的一个新功能，当一个化身处于示教状态下时，它自己将不会运动，由你通过键盘的`上下左右键`移动它，
化身会将你的移动当做它自己的运动，并在这个过程中去学习。通过这种方式，你可以“示范”给它看，让它知道怎样做比较好。取消示范后，化身回到自主行动的状态。
3. **分享设置(Share)**，这与配置对话框中的设置一样，不再累述。
4. **学习模式(Learning Mode)**，也和配置对话框中的一样。
5. **记忆存取(Memory)**，每个化身都有自己的记忆，学习和行动的过程就是构建记忆和运用记忆的过程。GAMCS提供了接口可以对记忆进行存取，你可以讲一个学的比较好的化身的记忆存起来，
再将它加载到其他化身中，这样其他化身就瞬间拥有了那个化身所有的记忆和知识。

## 化身的学习和行为
GAMCS-Lab 的主要功能就是作为一个试验场来研究GAM模型中化身的学习和行为能力。

虽然化身的功能很简单，仅能感受x,y坐标，动作仅有五种。但是根据试验，目前已发现了很多有意思的现象，例如：

- _基本的学习、记忆能力_。化身对遇到的奶酪或刺钉等具有记忆功能。
可以用[Demo Scenes]中的`basic`或`maze`来试验。化身找到奶酪后会被其吸引，一直吃直到吃完，把它拖到原来的位置上它会立刻冲向奶酪。
- _动态学习能力_。当环境发生变化，例如某处的奶酪被吃完了或者原来的路被堵住了等等，化身会根据情况不断学习。
可以用[Demo Scenes]中的`bridges`来试验，待化身找到多条到奶酪的路之后，堵住它选择的那一条，被堵之后经过几次尝试它就会选择知道的另一条路。
- _最优/最佳路径能力_。如果化身知道有多条路可以到达奶酪，它总能选择最短的。如果有多条路可以到达奶酪，但是其中某些路上布满刺钉，化身总能选择最佳的路线。
并且根据上一条，这种判断是可以动态学习的。可以用`bridges`来试验，在化身走的那条路上放置几个刺钉，再把它拖到起点，
当它走那条路被刺几次后，它就会选择其他的路。
- _交流加速群体的学习_。一个个体找到奶酪后可以通知其他个体，最终大家都能很快的发现奶酪。
可以用中的`maze`试验，在起点放置10个老鼠，打开交流功能，范围设置大一点，频率调高，观察它们的行为。
- _好奇心_，化身具有探索未知环境的好奇心，它会选择优先探索自己未去过的地方。
这一点是在GAMCS-Lab中对每种化身硬编码进去的，说明用GAMCS来实现动物的某些本能行为是如此简单（可以查看源代码，实现就几行）。
好奇心可以画一个经典的`T-Maze`场景来验证。或者从`maze`中的老鼠的行为也可以清楚的看到。
- 等等，其他很多有趣的行为等待你来发现～

能模拟出这些基本但简单的学习和行为能力本身并不太让人吃惊，你可以在代码里面针对每种化身的特点写满各种条件判断，也许同样可以表现出类似的甚至更有趣的学习现象。

在GAMCS-Lab里，所有的化身，无论是老鼠、猫、大象，或者以后会加入的更复杂的化身，它们都是由GAMCS这样一个非常通用的模型来驱动的。
你可以翻看GAMCS-Lab的源代码，处理化身行为逻辑相关的代码基本只有GAMCS提供的那四个函数，而且每个都非常简单。

另一方面，在GAMCS里，它根本就不知道某个化身究竟是什么、老鼠还是猫、喜欢奶酪还是刺钉，所有输到模型里的东西都是纯数，整数或者浮点数，不带任何实际意义。
换句话说，它是一个纯数学模型，而就是这样一个简单的纯数学的东西竟然可以产生如此丰富的生物学现象，这才是真正让人惊叹的地方。