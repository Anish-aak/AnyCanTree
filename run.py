from mlxtend.preprocessing import TransactionEncoder
from mlxtend.frequent_patterns import fpgrowth, apriori, association_rules
import pandas as pd

# Read the transactions from each file
def read_transactions(file_path):
    with open(file_path, 'r') as file:
        transactions = [line.strip().split() for line in file.readlines()]
        transactions = [[int(item) for item in transaction] for transaction in transactions]
    return transactions

# Convert transactions to one-hot encoded dataframe
def transactions_to_df(transactions):
    te = TransactionEncoder()
    te_ary = te.fit(transactions).transform(transactions)
    df = pd.DataFrame(te_ary, columns=te.columns_)
    return df

# Apply FP-Growth algorithm to get frequent itemsets
def get_frequent_itemsets(df):
    frequent_itemsets = fpgrowth(df, min_support=0.1, use_colnames=True)
    return frequent_itemsets

# Generate rules using Apriori algorithm
def generate_rules(frequent_itemsets):
    rules = association_rules(frequent_itemsets, metric="confidence", min_threshold=0.5)
    return rules

# Calculate precision, recall, and F1
def calculate_metrics_itemsets(original_itemsets_df, res_itemsets_df):
    # Convert itemsets DataFrames to sets for comparison
    original_itemsets_set = set(frozenset(itemset) for itemset in original_itemsets_df['itemsets'])
    res_itemsets_set = set(frozenset(itemset) for itemset in res_itemsets_df['itemsets'])
    
    # True Positives (TP): Itemsets present in both original and res
    TP = len(original_itemsets_set.intersection(res_itemsets_set))
    # False Positives (FP): Itemsets present in res but not in original
    FP = len(res_itemsets_set - original_itemsets_set)
    # False Negatives (FN): Itemsets present in original but not in res
    FN = len(original_itemsets_set - res_itemsets_set)
    
    # Compute precision, recall, and F1
    precision = TP / (TP + FP) if (TP + FP) > 0 else 0
    recall = TP / (TP + FN) if (TP + FN) > 0 else 0
    F1 = 2 * (precision * recall) / (precision + recall) if (precision + recall) > 0 else 0
    
    return precision, recall, F1

# Read transactions
original_transactions = read_transactions('sortedip.txt')
res_transactions = read_transactions('res.txt')

# Convert to dataframes
original_df = transactions_to_df(original_transactions)
res_df = transactions_to_df(res_transactions)

# Get frequent itemsets
original_frequent_itemsets = get_frequent_itemsets(original_df)
res_frequent_itemsets = get_frequent_itemsets(res_df)

# Generate rules
original_rules = generate_rules(original_frequent_itemsets)
res_rules = generate_rules(res_frequent_itemsets)

# Calculate metrics
precision_itemsets, recall_itemsets, F1_itemsets = calculate_metrics_itemsets(original_frequent_itemsets, res_frequent_itemsets)

print(f"Precision for Itemsets: {precision_itemsets}")
print(f"Recall for Itemsets: {recall_itemsets}")
print(f"F1 for Itemsets: {F1_itemsets}")
