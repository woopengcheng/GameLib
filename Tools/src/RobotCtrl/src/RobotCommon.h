#pragma once
#include "CUtil/inc/Common.h"

const int cnBtnXLengthSpace		= 5;
const int cnBtnYLengthSpace		= 7;
const int cnTabYPosOffset			= 21;
const int cnBtnXLength = 30;
const int cnBtnYLength = 21;
const int cst_btn_id = 10000;
const int MAX_ROBOT_BTN_NUM = 2000;

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