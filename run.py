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

# Calculate precision, recall, and accuracy
def calculate_metrics(original_rules, res_rules):
    # True Positives (TP): Rules present in both original and res
    TP = len(set(res_rules).intersection(set(original_rules)))
    # False Positives (FP): Rules present in res but not in original
    FP = len(set(res_rules) - set(original_rules))
    # False Negatives (FN): Rules present in original but not in res
    FN = len(set(original_rules) - set(res_rules))
    
    precision = TP / (TP + FP) if (TP + FP) > 0 else 0
    recall = TP / (TP + FN) if (TP + FN) > 0 else 0
    accuracy = TP / (TP + FP + FN) if (TP + FP + FN) > 0 else 0
    
    return precision, recall, accuracy

# Read transactions
original_transactions = read_transactions('ip.txt')
res_transactions = read_transactions('res.txt')

# Convert to dataframes
original_df = transactions_to_df(original_transactions)
res_df = transactions_to_df(res_transactions)



# Get frequent itemsets
original_frequent_itemsets = get_frequent_itemsets(original_df)
res_frequent_itemsets = get_frequent_itemsets(res_df)

print(res_frequent_itemsets)

# Generate rules
original_rules = generate_rules(original_frequent_itemsets)
res_rules = generate_rules(res_frequent_itemsets)

# Calculate metrics
precision, recall, accuracy = calculate_metrics(original_rules, res_rules)
# precision, recall, accuracy = calculate_metrics(original_frequent_itemsets, res_frequent_itemsets)
# print(original_frequent_itemsets)
# print("GAAAAAPPP")
# print(res_frequent_itemsets)

print(f"Precision: {precision}")
print(f"Recall: {recall}")
print(f"Accuracy: {accuracy}")
