import sys
import csv
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix
from sklearn.neural_network import MLPClassifier

TEST_SIZE = 0.3 # Proportion of data used for testing
K = 3  # Number of nearest neighbors for k-NN classifier


class NN:
    def __init__(self, trainingFeatures, trainingLabels):
        self.trainingFeatures = trainingFeatures
        self.trainingLabels = trainingLabels

    def predict(self, features, k):
        predictions = []
        for feature in features:
            # Calculate Euclidean distances between feature and trainingFeatures
            distances = np.linalg.norm(self.trainingFeatures - feature, axis=1)
            # Get indices of k nearest neighbors
            k_indices = np.argsort(distances)[:k]
            # Get labels of k nearest neighbors
            k_labels = self.trainingLabels[k_indices]
            # Count unique labels and choose label with maximum count as prediction
            unique_labels, counts = np.unique(k_labels, return_counts=True)
            predictions.append(unique_labels[np.argmax(counts)])
        return predictions


def load_data(filename):
    features = []
    labels = []
    with open(filename, 'r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            features.append([float(val) for val in row[:-1]])  # Extract features from each row, convert to float
            labels.append(int(row[-1]))  # Extract label from last column, convert to int
    return np.array(features), np.array(labels)  # Convert features and labels to NumPy arrays


def preprocess(features):
    means = np.mean(features, axis=0)  # Calculate mean of each feature
    stds = np.std(features, axis=0)  # Calculate standard deviation of each feature
    normalized_features = (features - means) / stds  # Normalize features using (fi - fi_mean) / fi_std
    return normalized_features


def train_mlp_model(features, labels):
    mlp = MLPClassifier(hidden_layer_sizes=(10, 5), activation='logistic', random_state=42, max_iter=5000)
    mlp.fit(features, labels)
    return mlp


def evaluate(labels, predictions):
    accuracy = accuracy_score(labels, predictions)  # Calculate accuracy
    precision = precision_score(labels, predictions)  # Calculate precision

    # Check if positive class is present in the predictions
    if 1 in predictions:
        recall = recall_score(labels, predictions)  # Calculate recall
        f1 = f1_score(labels, predictions)  # Calculate F1-score
    else:
        recall = 0.0
        f1 = 0.0

    return accuracy, precision, recall, f1


def main():
    # Check command-line arguments
    if len(sys.argv) != 2:
        sys.exit("Usage: python email_classifier.py spambase.csv")

    # Load data from CSV file and split into train and test sets
    features, labels = load_data(sys.argv[1])
    features = preprocess(features)
    X_train, X_test, y_train, y_test = train_test_split(features, labels, test_size=TEST_SIZE, random_state=42)

    # Train a k-NN model and make predictions
    model_nn = NN(X_train, y_train)
    predictions_nn = model_nn.predict(X_test, K)
    accuracy_nn, precision_nn, recall_nn, f1_nn = evaluate(y_test, predictions_nn)

    # Print 1-NN results
    print("** K-Nearest Neighbor Results **")
    print("Accuracy: ", accuracy_nn)
    print("Precision: ", precision_nn)
    print("Recall: ", recall_nn)
    print("F1: ", f1_nn)

    # Compute and print confusion matrix for 1-NN
    cm_nn = confusion_matrix(y_test, predictions_nn)
    print("Confusion Matrix - K-NN:")
    print(cm_nn)

    # Train an MLP model and make predictions
    model_mlp = train_mlp_model(X_train, y_train)
    predictions_mlp = model_mlp.predict(X_test)
    accuracy_mlp, precision_mlp, recall_mlp, f1_mlp = evaluate(y_test, predictions_mlp)

    # Print MLP results
    print("** MLP Results **")
    print("Accuracy: ", accuracy_mlp)
    print("Precision: ", precision_mlp)
    print("Recall: ", recall_mlp)
    print("F1: ", f1_mlp)

    # Compute and print confusion matrix for MLP
    cm_mlp = confusion_matrix(y_test, predictions_mlp)
    print("Confusion Matrix - MLP:")
    print(cm_mlp)


if __name__ == "__main__":
    main()
