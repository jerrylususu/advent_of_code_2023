# Advent of Code 2023

**⚠警告**：很糟糕的代码，只是为了把结果算出来，没有任何优化


语言：Python (Jupyter Notebook)，如果不够快的话会转成 C++

工具：Copliot

结果：49 ⭐（day21 part2实在是没做出来，抄了别人的题解）



## 难度评级

| 难度                                | 题目（所在天数）                             |
| ----------------------------------- | -------------------------------------------- |
| SSR （没做出来）                    | 21                                           |
| SR （做了很久 / 暴力解 / 看了思路） | 5, 8, 10, 12, 16, 17, 18, 19, 20, 23, 24, 25 |
| R（得仔细想想）                     | 1, 3, 13, 14                                 |
| N（有手就行）                       | 2, 6, 7, 9, 11, 15, 22                       |



## 每日记录

> day 1 ~ day 10 是一个周末肝出来的，所以不太完整，没记录的大概是认为当天比较简单

- day 1: p2 得 rfind，直接 index 比较似乎有问题
- day 3: off by 1 error 
- day 5: p2太暴力了，应该按照分段来会更好一些（或者就算暴力的话应该要高度并行化，虽然我是硬跑了4线程*1.5小时跑出来的）
- day 8: p2太暴力了，最后果然是环+lcm（这里查了下别人的解析)，暴力跑必然是跑不出来的
- day 10: 解析图有点烦人，而且还得懂点计算机图形学？（ray casting 判断区域内外）

> day 11 开始每日跟着做

- day 12: p1可以暴力过，但是p2...不dp就不行了，幻想用五倍关系来搞是没用的，还是直接全量dp吧
- day 13: p2得考虑多个对称轴的情况，之前的抽象不太合理导致卡了好久
- day 14: 比较直接，p1直接模拟，p2这么大的数直接猜有循环，不过最后发现还是off by 1了，好在-1之后就对了
- day 15: 今天的很直接，虽然p2文字描述多了点，读懂了还是很快的，比较简单
- day 16: 相对直接，但是实现上得小心点；p1直接模拟了，用个队列存当前的光线坐标和方向，需要注意判重；p2直接硬算了（not my best moment），好在量不大+4并行，多等等就出来了（还顺带试了copliot把py转cpp)
- day 17: 终究还是逃不过算法了，p1用dijkstra，但是生成下一步的时候要加上三步限制；p2在p1上魔改，实际上是支持1步/4步的step size，生成的时候步数限制也得改下（说来惭愧，毕业之后算法几乎已经忘光了，自己整了一个小时没整出来，最后求助copliot chat才做出来，作为程序员有些失败）
- day 18: 难度又创新高, p1本来想ray casting解，但是因为边本身有宽度不太好搞，最后直接flooding了；p2数据量太大，光是存grid都不太可能了，和gpt4聊了一会之后学到了一个叫shoelace formula的神奇操作，但是这也只能解决边界内的面积，边界上的面积还要再算下，好在经过一些基础的几何推导可以发现边界上需要计算的面积实际上就是 边长/2 + 1（可以通过内角->外角推断出来）
  - P.S 后来学习到了 [Pick's theorem](https://en.wikipedia.org/wiki/Pick's_theorem)
- day 19: 总算回到了能做的难度；p1很直接，模拟就是了；p2硬算的话有2^48，那必然没法暴力了；想着说建个图看看，但是又担心层级太多；后来把每个workflow拆成了几个节点，每个节点有自己的条件列表（例如wf1的第三个节点，条件是 x<50 + y<80 + z>=1000），再bfs一把找到所有可以到达A的路径（in->wf1->wf2->A），发现其实也就500多条；将路径里所有的条件AND，对每个维度算一次区间大小，再相乘就是当前路径可达A的数量，最后所有路径求和就可以了
- day 20: 题目描述字数又创新高，感觉读了得有十分钟才弄明白；p1 直接模拟，写完发现自己居然在 top1k；p2 看了问题，一开始没啥想法，感觉硬算都不可能；一开始先跑了1亿，跑着跑着想到就作者这个出题风格，暴力估计还是解不出来；后来仔细看了下数据，发现最后实际上就是四个AND，盲猜带周期+最小公倍数，最后果然如此
- day 21: 投降了，真做不出来了；p1 依然是正常的模拟；p2 完全没有头绪；题目的这个数字看起来意外的具体，显然是有一些特殊做法，但是完全想不到，甚至都试着拟合多项式了；虽然想到了应该得分奇偶块，然而完全没法用上；一开始感觉是个圆，后来看了可视化动图才发现是个菱形，然而依然感觉搞不出来；最后对着一个油管题解抄了
  - 虽然数据的确很特殊（中心开始，边缘/中心轴线没有障碍），但是计算的时候要分很多块，太容易 off by 1了，就算出错了估计都查不出来...
  - 据说还真的可以二次拟合？但是需要很特定的x（65, 65+131, 65+131*2）
- day 22: 意外的直接，p1 p2 都是直接模拟可过，难道是作者良心发现昨天太难了？ (bgm38) 一开始还以为p2要搞什么组合最小DP之类的花活，但是并没有
- day 23: 没啥特别好的想法，p1 p2 都是直接dfs，到终点了就尝试更新最大值；p2用cpp重写之后去吃了个饭，0.5b step之后最大值就不变了，提交上去也ac了；有些好奇正规解法是什么
  - P.S. 看了别人的题解，可以观察到题目中大部分位置其实都只有一个可选的前进方向，在图上可以把这类点忽略（or 合并，不太确定 coalesce 怎么翻译合适），这样可以大幅减小最后需要暴力的图上边的数量
- day 24: 相对直接？ p1 用一些初中几何就好（一开始读题不仔细以为真的要相撞，搞了半天才发现是轨迹相交）; p2 样例里有平行直线，写了个几何解，但是发现数据里没有，就卡着了，差点投降了；后来突然想起来可以用sympy列方程硬解（三条直线上设3个未知数t1 t2 t3，计算出三条线上的三个点坐标，然后要求这三个点在同一直线上），居然就硬解出来了 (bgm38)
  - P.S. 看了看别人的题解，基本上 p2 都是用数学库解方程了（理论上可以手写高斯消元？）
- day 25: 最后一天！p1 想了会没想出来比较好的解法，于是干脆把图导入到gephi里直接可视化跑了下，直接可以看出来两个大点群，框选计数就得到答案了；p2 直接点一下就可以了
  - P.S 看了其他人的题解，主要还是用 networkx 之类的图计算库，不过似乎专门的算法 / bfs 点对抽样也可以接出来
