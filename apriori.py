# 导入所需模块
from collections import defaultdict
from itertools import combinations
import pandas as pd
import time
import sys

class HashTreeNode:
    def __init__(self, depth=0, max_depth=3):
        self.depth = depth
        self.max_depth = max_depth
        self.children = {}
        self.is_leaf = depth == max_depth
        self.items = set()

    def insert(self, itemset):
        if self.is_leaf:
            self.items.add(itemset)
            return

        key = hash(frozenset(itemset)) % 3
        if key not in self.children:
            self.children[key] = HashTreeNode(self.depth + 1, self.max_depth)

        self.children[key].insert(itemset)

    def get_itemsets(self, transaction, itemsets):
        if self.is_leaf:
            for item in self.items:
                if item.issubset(transaction):
                    itemsets.add(item)
            return

        for key, child in self.children.items():
            child.get_itemsets(transaction, itemsets)


# 定义Apriori类
class Apriori:
    def __init__(self, transactions, min_support, min_confidence):
        self.transactions = transactions
        self.min_support = min_support
        self.min_confidence = min_confidence
        self.item_set = self.get_item_set()
        self.frequent_itemsets = {}
        self.rules = []

    def get_item_set(self):
        item_set = set()
        for transaction in self.transactions:
            for item in transaction:
                item_set.add(frozenset([item]))
        return item_set

    def run(self):
        k = 1
        current_l_set = self.item_set
        while current_l_set:
            self.frequent_itemsets[k] = current_l_set
            current_l_set = self.join_set(current_l_set, k)
            current_l_set = self.get_support(current_l_set)
            k += 1

        self.generate_rules()

    def join_set(self, item_set, k):
        return set([i.union(j) for i in item_set for j in item_set if len(i.union(j)) == k + 1])

    def get_support(self, item_set):
        hash_tree = HashTreeNode()
        for item in item_set:
            hash_tree.insert(item)

        item_count = defaultdict(int)
        for transaction in self.transactions:
            matching_itemsets = set()
            hash_tree.get_itemsets(transaction, matching_itemsets)
            for item in matching_itemsets:
                item_count[item] += 1

        return set(item for item in item_count if item_count[item] >= self.min_support * len(self.transactions))

    def generate_rules(self):
        for key, value in self.frequent_itemsets.items():
            if key == 1:
                continue
            for item_set in value:
                _subsets = [frozenset(x) for r in range(1, len(item_set)) for x in combinations(item_set, r)]
                for element in _subsets:
                    remain = item_set.difference(element)
                    if remain:
                        confidence = self.get_confidence(element, item_set)
                        if confidence >= self.min_confidence:
                            self.rules.append((element, remain, confidence))

    def get_confidence(self, subset, parent_set):
        parent_support = self.get_item_support(parent_set)
        subset_support = self.get_item_support(subset)
        return parent_support / subset_support

    def get_item_support(self, item):
        support = 0
        for transaction in self.transactions:
            if item.issubset(transaction):
                support += 1
        return support / len(self.transactions)


def print_frequent_itemsets(frequent_itemsets):
    for k, itemsets in frequent_itemsets.items():
        print(f"频繁{k}项集：")
        for itemset in itemsets:
            items = ', '.join(itemset)
            print(f"  - {items}")


if __name__ == '__main__':
    # 加载数据
    file_path = 'Groceries_dataset.csv'
    groceries_data = pd.read_csv(file_path,nrows=10000)
    # 按会员编号和日期分组，然后创建交易列表
    transactions = groceries_data.groupby(['Member_number', 'Date'])['itemDescription'].apply(list).tolist()

    # 创建Apriori实例
    apriori_instance = Apriori(transactions, min_support=0.008, min_confidence=0.1)

     # 测量运行时间
    start_time = time.time()

    # 运行Apriori算法
    apriori_instance.run()

    end_time = time.time()

    # 打印生成的规则
    print("关联规则:")
    for rule in apriori_instance.rules:
        antecedent = ', '.join(rule[0])  # 将前件转换为逗号分隔的字符串
        consequent = ', '.join(rule[1])  # 将后件转换为逗号分隔的字符串
        confidence = rule[2]             # 置信度
        print(f"({antecedent}, {consequent}, confidence={confidence})")
    print_frequent_itemsets(apriori_instance.frequent_itemsets)


    print("运行时间:", end_time - start_time, "秒")

    # 测量内存消耗
    print("大致内存消耗:", sys.getsizeof(apriori_instance), "字节")


    # 计算运行时间
    execution_time = end_time - start_time

    # 估计内存消耗
    memory_usage = sys.getsizeof(apriori_instance)

    # 打开一个文本文件用于写入结果
    with open('apriori_results_support=0.01_confidence=0.1_test.txt', 'w') as file:
        # 写入频繁项集
        file.write("频繁项集:\n")
        for k, itemsets in apriori_instance.frequent_itemsets.items():
            file.write(f"长度为 {k} 的频繁项集:\n")
            for itemset in itemsets:
                file.write(f"{', '.join(itemset)}\n")
        file.write("\n")

        # 写入关联规则
        file.write("关联规则:\n")
        for rule in apriori_instance.rules:
            antecedent = ', '.join(rule[0])
            consequent = ', '.join(rule[1])
            confidence = rule[2]
            file.write(f"({antecedent} -> {consequent}, confidence={confidence:.2f})\n")
        file.write("\n")

        # 写入运行时间和空间消耗
        file.write(f"运行时间: {execution_time} 秒\n")
        file.write(f"估计内存消耗: {memory_usage} 字节\n")

    # 通知用户结果已保存
    print("结果已保存到 apriori_results.txt")

