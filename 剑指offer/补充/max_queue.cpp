#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cassert>
#include <deque>
using namespace std;

/**
 * 实现最大队列，使得获取队列当前最大值，插入队列，删除队列的均摊时间复杂度都为O(1)
 * 相比于利用两个栈实现队列和利用两个队列实现栈（比较简单能想到）以及最大栈和最小栈
 * 最大队列会难一点，很容易想到利用辅助队列，维护当前栈中的最大值
 * 那么辅助栈中的应该选择哪些元素呢？首先队列是先进先出的数据结构
 * 辅助队列中的首个元素一定是原队列中的最大元素，这样能保证以较低的时间复杂度获取max元素(不用遍历)
 * 其次，如果原数据队列中的最大元素被pop_front，如何获取下一个最大元素（也就是最大元素还没有出队时的第二大）？
 * 因此，关键是辅助队列中应该插入什么元素
 * 考虑队列中最大元素的影响，如果当前某个位置时最大元素，那么由于队列先进先出的性质，在最大元素之前入队的那些元素，不管何时出队
 * 如果后续没有更大元素进队，那么那些元素的处队也不会影响到整个队列的最大元素，加入有一个更大的元素入队，则最大元素只会更大，直到当前的最大元素也出队
 * 才可能发生最大元素的变化，因此，排在最大元素之前的那些元素，永远不可能成为最大元素了
 * 相反，在最大元素之后的元素，在最大元素出队后，则有成为最大元素的可能性
 * 1. 考虑当前的队列中只有一个元素，那么此时辅助队列中应该也只有这个元素，表示当前的最大元素
 * 2. 此时插入一个更小的元素，这个元素也应该被插入到辅助队列，因为有可能成为在首个元素出队之后的最大元素
 * 3. 此时插入一个比首元素更大的元素，这个元素显然会成为当前最大元素，并且前两个元素即使出队，也不会影响到最大元素的变更
 * 4. 因此辅助队列中不需要前两个元素了，因为它们不可能成为最大元素了，将辅助队列中比插入元素更小的元素全部移除，直到队列为空（或者辅助队列中还有一个更大的元素）
 * 5. 此时插入一个很小的元素，比辅助队列中的尾元素更小，由于这个元素出现在后面，因此具有成为最大元素的可能，也将其插入到辅助队列的尾部
 * 5. 此时再插入一个比上一个元素稍微大的元素，我们直到，这个元素也是有成为最大元素的可能的，显然要插入辅助队列
 * 6. 但因为这个元素的插入，使得数据列中在此元素之前出现的，并且比它小的元素失去了成为最大元素的可能，因此辅助队列中保存的比这个元素小的元素都应该删去，直到遇到一个更大的元素
 * 7. 辅助队列实际上是一个单调递减的队列
 * 8. 如上，便得到了问题的解法
*/

class MaxQueue {
public:
	MaxQueue();
	int max_value();
	void push_back(int value);
	int pop_front();
private:
	queue<int> q;
	deque<int> help;
};

MaxQueue::MaxQueue() {}

int MaxQueue::max_value() {
	assert(!q.empty());
	assert(!help.empty());
	return help.front();
}

void MaxQueue::push_back(int value) {
	while (!help.empty() && help.back() < value)
		help.pop_back();
	q.push(value);
	help.push_back(value);
}

int MaxQueue::pop_front() {
	assert(!q.empty());
	int ret = q.front();
	q.pop();
	if (ret == help.front())
		help.pop_front();
	return ret;
}