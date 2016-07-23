#pragma once
#include "CUtil/inc/Common.h"

const INT32 cnBtnXLengthSpace		= 5;		//5 机器人按钮的X轴的空隙长度
const INT32 cnBtnYLengthSpace		= 7;		//5 机器人按钮的X轴的空隙长度
const INT32 cnTabYPosOffset			= 21;		//5 对话框的tab标签的高度偏移
const INT32 cnBtnXLength			= 30;		//5 机器人的按钮X轴长度			
const INT32 cnBtnYLength			= 21;		//5 机器人的按钮Y轴长度
const INT32 cst_btn_id				= 10000;	//5 机器人的按钮ID从哪个数开始.
const INT32 cnMaxRobotBtnNum		= 2000;		//5 初始创建机器人按钮的个数.
const INT32 cnColumnWidth			= 60;		//5 显示机器人每一列的宽度
const INT32 cnMinRobotDlgWidth		= cnColumnWidth * 7;	//5 对话框的最小宽度
const INT32 cnRobotStartID			= 10000;	//5 机器人的序号从多少开始.

//5 替换简单的Map某个Key排序后的右边的值.一般是由于某个Key被删除了,后面的会依次前移.第一要保证有序的map,第二不需要考虑释放的map
template<typename MAP , typename Key>
void		ReplaceSimpleMapRightKey(MAP &map1, MAP &map2 , Key key)
{
	MAP temp;
	MAP::iterator iter = map1.find(key);
	if (iter != map1.end())
	{
		if (iter != map1.begin())
		{
			temp.insert(map1.begin(), iter);
		}
		
		for (++iter; iter != map1.end(); ++iter)
		{
			key = iter->first;
			temp.insert(std::make_pair(key - 1, iter->second));
		}

		if (temp.size() >= 0)
		{
			map2.clear();
			map2.swap(temp);
		}
	}
	temp.clear();
}

//5 上面的函数替换后,和他相对的也需要替换.
template<typename MAP1, typename MAP2>
void		ResetMapKey(MAP1 & map1, MAP2 & map2)
{
	map2.clear();
	MAP1::iterator iter = map1.begin();
	for (; iter != map1.end();++iter)
	{
		map2.insert(std::make_pair(iter->second , iter->first));
	}
}