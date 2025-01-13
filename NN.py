import numpy as np
import tensorflow as tf
from tensorflow.keras.callbacks import Callback
import sys
import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'

from tensorflow.keras.utils import plot_model

# Prédiction sur de nouvelles données
sequencesTest = [
    ['a----', '----b', '--k--', '-----', '-b---', '-----', '----a', '--k--'],
['-----', '--ak-', '-----', '---b-', '-----', '-----', '-a--b', '----k'],
['---b-', '-----', 'a-k--', '-----', '---b-', '--k--', '-----', '-a---'],
['--a--', '-----', '---k-', '-b---', '-----', '-k-a-', '-b---', '-----'],
['-k---', '-b---', '----a', '-----', '-----', '--a--', 'b---k', '-----'],
['--k--', '--a--', '-----', '---b-', '---a-', 'bk---', '-----', '-----'],
['-----', '-a-b-', '---k-', '-----', '-a---', '-----', '---k-', '----b'],
['-----', '-a--b', '-----', '-k---', 'a----', '--b--', '-----', '--k--'],
['-----', '-ab--', '-----', '-----', '-----', 'a----', '--b--', 'k----'],
['-----', '-----', '-b---', '-a--k', '--bka', '-----', '-----', '-----'],
['a----', '-----', '-----', '-k---', 'k----', '--a--', '-----', '-----'],
['-----', '----k', '-----', 'a---b', '-----', '--bk-', 'a----', '-----'],
['-----', '-----', '---kb', '----a', '---k-', '-----', '-a---', '---b-'],
['k----', '-----', '-b--a', '-----', '-----', '-----', '--k--', '---ba'],
['---b-', '----a', '-k---', '-----', '-----', '--b--', '--k--', 'a----'],
['----b', '-----', 'k--a-', '-----', '-----', '-----', '-----', 'b-k-a'],
['----k', '-----', '-----', '---b-', '----a', '-----', '----b', 'k----'],
['-----', '---a-', '----b', '----k', '-----', '--b--', '---a-', 'k----'],
['---ab', 'k----', '-----', '-----', '--b--', '-----', '--k--', '--a--'],
['-----', '-b-k-', '-a---', '-----', '---b-', '----a', '-k---', '-----'],
['--k--', 'b----', '-----', '-a---', '-----', '-k---', '-----', '--ab-'],
['---a-', '-----', '-b---', '---k-', '-----', '----k', '-a---', '-b---'],
['-----', '---k-', '---a-', '-----', '---k-', '--b--', '-----', 'a----'],
['--a--', '-----', '---b-', '----k', '-----', '-----', '----a', '-bk--'],
['-----', '-----', 'b---k', '--a--', '-k---', '-----', '---b-', '--a--'],
['-----', '-ak--', '-----', '-b---', '-----', '--b--', '-a---', 'k----'],
['-----', 'a---k', '-----', 'b----', '-----', '-----', '---k-', '-b---'],
['---bk', '-a---', '-----', '-----', '-----', '---ka', '---b-', '-----'],
['--k--', '-a---', '-----', '----b', '-----', '---ab', 'k----', '-----'],
['k----', 'a----', '-----', '----b', 'b--k-', '--a--', '-----', '-----'],
['--a--', 'b----', '-----', '----k', '----a', '-----', '-b---', '-----'],
['-----', '--a--', '---b-', '--k--', '----a', 'b----', '-----', 'k----'],
['--a--', '--b--', '--k--', '-----', '---k-', '-----', '-----', '--a--'],
['-----', '-----', 'b----', 'a-k--', '-b-a-', '-----', '-----', 'k----'],
['----a', 'b----', '-----', '-k---', '----a', '-----', '----b', '---k-'],
['-k---', '---b-', 'a----', '-----', '-k---', '-----', '--ab-', '-----'],
['----k', '-----', '----a', '-----', '-----', '-b--k', '----a', '-----'],
['-a--b', '-----', '---k-', '-----', '-----', 'a----', '--k--', '-----'],
['-----', '---a-', '-b---', 'k----', '--b--', '-----', '--a--', '----k'],
['-----', '-----', 'k----', 'b-a--', '-----', '-----', 'k-a--', '---b-'],
['-----', '-k---', '---b-', '----a', '-----', 'k----', '--a--', '--b--'],
['k----', '-----', '----b', '--a--', '---a-', '--k--', '-----', 'b----'],
['-----', '-----', 'a----', 'b---k', '--a--', 'k----', '--b--', '-----'],
['-k---', 'a----', '----b', '-----', '-----', '--k-b', '-----', '----a'],
['-----', '----k', '-----', '--ba-', 'a----', '-----', '--k--', '-b---'],
['b----', '-----', '-----', '--a-k', '-----', '-b---', '-k--a', '-----'],
['-a--k', '-----', '-----', 'b----', '---a-', '-k---', '-b---', '-----'],
['-----', 'ak---', '-----', 'b----', '-----', 'a----', '----k', '-b---'],
['-----', 'k----', '-----', '--b--', '-----', '---a-', '-b---', '----k'],
['---b-', '-----', '-----', '-ak--', '-----', '-k--b', 'a----', '-----'],
['-----', 'k--ab', '-----', '-----', '-k--b', '-----', '----a', '-----'],
['--a--', '-----', 'b-k--', '-----', '-----', '---b-', '-----', '----k'],
['-----', '----k', 'b----', 'a----', '-----', '----k', '-----', 'a--b-'],
['-----', '-----', '-b---', 'a-k--', '---k-', '-ab--', '-----', '-----'],
['----k', '-b---', '-a---', '-----', '--k--', '-----', '----a', '----b'],
['k-b--', '-----', 'a----', '-----', '-----', 'k----', '--a--', '-----'],
['-----', '-----', '---ab', '-k---', '-----', '----b', '-----', '-----'],
['---bk', '-----', '---a-', '-----', '--b--', '--k--', '-----', '---a-'],
['-----', 'k----', '-ba--', '-----', '-----', '----k', '-----', '-a-b-'],
['--k--', '-----', '-b---', '--a--', 'b-a--', '-----', '-----', 'k----'],
['a----', 'b----', '-k---', '-----', '---a-', '-----', '-----', '----k'],
['--ak-', '-----', '-----', '----b', '-----', '-kb--', '-----', '--a--'],
['-k---', '-----', '----a', '---b-', '---ak', '-----', '-b---', '-----'],
['k----', '-----', '-a---', '---b-', '-----', '----b', '-----', '---a-'],
['a----', '-----', '-k---', '--b--', '-----', '----b', '----k', '---a-'],
['---b-', '-----', '---a-', '-----', '-----', '-----', '---a-', '---kb'],
['-----', '-k---', '-----', '----b', '--a--', '-----', '-----', '---b-'],
['-k---', '--b--', '---a-', '-----', '-----', '--b--', 'a----', '----k'],
['-----', '--ab-', '-----', '---k-', '-----', '-a---', '--k--', '---b-'],
['-----', '-----', 'k----', '--b--', '----k', '--b--', '-----', '-----'],
['a----', 'k----', '-b---', '-----', '-----', '----a', '-b---', '-k---'],
['-----', '---b-', 'a----', 'k----', '-----', '---a-', 'k----', '---b-'],
['b----', '----k', '-----', 'a----', '-----', '--k--', '---a-', '-----'],
['-----', '--ak-', '---b-', '-----', '-----', 'b----', '-k---', '----a'],
['b----', '-k---', '---a-', '-----', '-----', '-----', 'k----', '-b---'],
['---b-', '-----', '----k', '----a', '-----', '-----', '-b--k', '----a'],
['-----', '---k-', '-a---', 'b----', '-----', '--a--', 'kb---', '-----'],
['-----', '-----', '----a', 'b---k', 'k----', '-b---', '-----', '---a-'],
['---b-', '-----', '-----', '----k', 'b----', '-a---', '---k-', '-----'],
['kb---', '-----', 'a----', '-----', '-----', '----b', '--a--', '-k---'],
['-----', '-k-b-', '--a--', '-----', '-----', '--k--', 'b--a-', '-----'],
['----a', '-----', '-----', 'k--b-', '-----', '-----', '----a', '--b--'],
['-----', 'b---a', '-----', 'k----', '-----', '-----', '---k-', '-a-b-'],
['---k-', '--ab-', '-----', '-----', '---b-', '--k-a', '-----', '-----'],
['-----', '-ka--', '-----', '--b--', '----a', '---b-', '-----', '-----'],
['-----', '-----', 'a----', 'b---k', 'k---b', '-----', '-a---', '-----'],
['-a---', '-----', '--b--', '----k', '-b---', '----k', '-----', '--a--'],
['--b--', '-----', '---k-', 'a----', '----b', '----k', '----a', '-----'],
['-----', 'b--k-', '-----', '----a', 'a----', 'b----', '---k-', '-----'],
['-----', '-b---', 'k----', '--a--', '---b-', '-----', '-ak--', '-----'],
['-----', '--k--', '-b---', '---a-', '-----', '-----', '-k--b', '-a---'],
['--k--', '-----', '----b', '-----', '----b', '---a-', '-----', '---k-'],
['-----', '-----', 'a----', '---kb', '---a-', '-----', '-----', 'b---k'],
['-----', 'kb---', '----a', '-----', '-----', '-k---', '-----', '---b-'],
['a----', '---b-', '---k-', '-----', '----a', '----k', '-----', '-b---'],
['-ka-b', '-----', '-----', '-----', '----k', '-b---', '-----', '--a--'],
['-----', '-b-k-', '---a-', '-----', 'k----', '-b---', '----a', '-----'],
['-----', '---k-', '--a--', '---b-', '--b--', '----k', '-----', '--a--'],
['-----', 'b----', 'a---k', '-----', '-----', '-----', '--ab-', '---k-'],
['-----', 'kb-a-', '-----', '-----', '-----', '----a', '---k-', 'b----']
]

expectedTest = ["deux", "deux", "un", "deux", "un", "un", "deux", "un", "deux", "deux", "deux", "deux", "deux", "un", "un", "un", "deux", "deux", "deux", "deux", "deux", "deux", "deux", "deux", "deux", "deux", "un", "un", "deux", "deux", "un", "deux", "un", "un", "deux", "deux", "deux", "un", "deux", "deux", "un", "un", "deux", "deux", "deux", "deux", "deux", "un", "deux", "un", "deux", "un", "deux", "deux", "deux", "un", "un", "deux", "deux", "deux", "un", "deux", "deux", "un", "deux", "deux", "un", "un", "deux", "deux", "deux", "deux", "un", "deux", "un", "deux", "un", "un", "deux", "deux", "deux", "un", "deux", "deux", "un", "deux", "deux", "deux", "deux", "deux", "deux", "deux", "un", "un", "un", "deux", "un", "deux", "deux", "deux"]




# Lecture des données à partir du fichier texte
def lire_donnees(filename):
    with open(filename, 'r') as file:
        data = file.read().splitlines()
    return data

# Prétraitement des données
def pretraiter_donnees(data):
    sequences = []
    labels = []
    current_sequence = []

    for line in data:
        if line == 'un' or line == 'deux':
            labels.append(line)
            sequences.append(current_sequence)
            current_sequence = []
        else:
            current_sequence.append(list(line))

    return sequences, labels

# Conversion des caractères en vecteurs one-hot
def encoder_donnees(sequences, labels):
    char_to_int = {'-': 0, 'a': 1, 'b': 2, 'k': 3}
    int_to_char = {0: '-', 1: 'a', 2: 'b', 3: 'k'}

    encoded_sequences = []
    encoded_labels = []

    for seq, label in zip(sequences, labels):
        encoded_seq = [[char_to_int[char] for char in row] for row in seq]
        encoded_sequences.append(encoded_seq)

        if label == 'un':
            encoded_labels.append([1, 0])  # [1, 0] représente 'un'
        else:
            encoded_labels.append([0, 1])  # [0, 1] représente 'deux'

    return np.array(encoded_sequences), np.array(encoded_labels)

# Définition du modèle
def creer_modele():
    model = tf.keras.Sequential([
        tf.keras.layers.Flatten(input_shape=(8, 5)),
        tf.keras.layers.Dense(32, activation='relu'),
        tf.keras.layers.Dense(2, activation='softmax')
    ])

    model.compile(optimizer='adam',
                  loss='categorical_crossentropy',
                  metrics=['accuracy'])

    return model



# Lecture des données à partir du fichier texte
def lire_data(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data


def convertir_document_vers_liste(document):
    lignes = document.strip().split('\n')
    resultList = []
    gameList = []
    liste = []
    for ligne in lignes:
        if len(ligne) == 5:
            liste.append(ligne)
        else:
            gameList.append(liste)
            resultList.append(ligne)
            liste = []
    return gameList, resultList


def testNN(new_sequences, expectedList):
    print("Batch of size:", len(expectedList))
    # Chargement des poids du modèle
    model.load_weights('fichier_de_poids.h5')
    
    encoded_new_sequences = encoder_donnees(new_sequences, ['un'] * len(new_sequences))[0]
    predictions = model.predict(encoded_new_sequences)
    predicted_labels = ['un' if pred[0] > 0.5 else 'deux' for pred in predictions]
    # Affichage des prédictions
    successRate = 0
    for sequence, label, expected in zip(new_sequences, predicted_labels, expectedList):
        if label == expected:
            successRate += 1
    print("The success rate is:", successRate*100/len(expectedList), "% on the tests.")


def createNN():
    model.fit(encoded_sequences, encoded_labels, epochs=10)

    # Sauvegarde des poids du modèle
    model.save_weights('fichier_de_poids.h5')
    print("Predefined tests:")
    testNN(sequencesTest, expectedTest)
    

def trainNN():
    # Chargement des poids du modèle
    model.load_weights('fichier_de_poids.h5')

    model.fit(encoded_sequences, encoded_labels, epochs=10)

    # Sauvegarde des poids du modèle
    model.save_weights('fichier_de_poids.h5')
    print("Predefined tests:")
    testNN(sequencesTest, expectedTest)
    

def testForResult():
    # Chargement des données
    data = lire_data(filename)
    sequences, expected = convertir_document_vers_liste(data)
    print("Test with 'results' file:")
    testNN(sequences, expected)


def predictGameInProgress(teamNumber):
    # Chargement des données
    data = lire_data("gameInProgress.txt")
    sequences, expected = convertir_document_vers_liste(data)
    # Chargement des poids du modèle
    model.load_weights('fichier_de_poids.h5')
    
    encoded_new_sequence = encoder_donnees(sequences, ['un'] * len(sequences))[0]
    predictions = model.predict(encoded_new_sequence)
    if teamNumber == "1":
        print("The win rate for Team 1 is: ", predictions[0][0]*100, "%.")
    else:
        print("The win rate for Team 2 is: ", (1-predictions[0][0])*100, "%.")

    

# Chargement des données
filename = 'resultats.txt'
data = lire_donnees(filename)
sequences, labels = pretraiter_donnees(data)
encoded_sequences, encoded_labels = encoder_donnees(sequences, labels)


# Création et entraînement du modèle
model = creer_modele()


def main():
    # Check if the parameter was provided
    if len(sys.argv) < 2:
        print("No parameter provided.")
        return

    # Get the parameter value
    parameter = sys.argv[1]
  
    if len(sys.argv) > 2:
        parameterTwo = sys.argv[2]

    if parameter == "S":
        createNN()
    elif parameter == "T":
        trainNN()
    elif parameter == "E":
        testForResult()
    elif parameter == "P":
        predictGameInProgress(parameterTwo)

main()



