# 弈子行为

## AITask（MovementTask）

创建了一个任务（AITask），用于控制弈子的寻路，并在移动过程中对路径进行及时更新。每进入一个格点进行一次，保证任务执行频率合理。

它应该受以下因素影响：

* 起点与终点棋格
* 弈子攻击范围
* 弈子当前目标

## AITask_Movement类设计

public：

创建AITask_Movement对象

获取AI的目标弈子、格点、攻击范围等

protected：

任务触发时执行，即开始移动

获取目标弈子当前格点与目标结点

任务成功完成时执行的函数

任务失败时执行的函数

引导弈子进行移动

AI目标弈子、棋格、攻击范围

任务成功时执行的委托

任务失败时执行的委托
