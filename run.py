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
def calculate_metrics(original_rules_df, res_rules_df):
    # Convert rules DataFrames to sets of frozensets for comparison
    original_rules_set = set(frozenset(tuple(rule)) for rule in original_rules_df[['antecedents', 'consequents']].itertuples(index=False, name=None))
    res_rules_set = set(frozenset(tuple(rule)) for rule in res_rules_df[['antecedents', 'consequents']].itertuples(index=False, name=None))
    
    # True Positives (TP): Rules present in both original and res
    TP = len(original_rules_set.intersection(res_rules_set))
    # False Positives (FP): Rules present in res but not in original
    FP = len(res_rules_set - original_rules_set)
    # False Negatives (FN): Rules present in original but not in res
    FN = len(original_rules_set - res_rules_set)
    
    # Compute precision, recall, and F1
    precision = TP / (TP + FP) if (TP + FP) > 0 else 0
    recall = TP / (TP + FN) if (TP + FN) > 0 else 0
    F1 = (2 * precision * recall)/(precision + recall) if (precision + recall) > 0 else 0
    
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
precision, recall, F1 = calculate_metrics(original_rules, res_rules)

print(f"Precision: {precision}")
print(f"Recall: {recall}")
print(f"F1: {F1}")
