#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tensorflow/c/c_api.h>

// Fonction pour sauvegarder les poids du modèle
void saveModel(TF_Graph* graph, TF_Session* session, const char* filename) {
    TF_Status* status = TF_NewStatus();
    TF_Buffer* buffer = TF_NewBuffer();
    TF_SessionOptions* options = TF_NewSessionOptions();

    // Créer un contexte d'enregistrement du modèle
    TF_Saver* saver = TF_NewSaver(graph, options);

    // Enregistrer les poids du modèle dans un buffer
    TF_SaveSession(session, options, buffer, status, saver, filename, NULL);

    // Écrire le contenu du buffer dans un fichier
    FILE* file = fopen(filename, "wb");
    fwrite(buffer->data, sizeof(char), buffer->length, file);
    fclose(file);

    // Nettoyage des ressources
    TF_DeleteBuffer(buffer);
    TF_DeleteSessionOptions(options);
    TF_DeleteStatus(status);
    TF_DeleteSaver(saver);
}

// Création du réseau de neurones
TF_Session* createNeuralNetwork() {
    // Définir la configuration du réseau de neurones
    TF_Graph* graph = TF_NewGraph();
    TF_SessionOptions* options = TF_NewSessionOptions();
    TF_Buffer* graphDef = TF_NewBuffer();

    // Chargement du modèle pré-entrainé
    const char* filename = "path/to/pretrained_model.pb";
    FILE* file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* data = (char*)malloc(fsize + 1);
    fread(data, sizeof(char), fsize, file);
    fclose(file);

    graphDef->data = data;
    graphDef->length = fsize;

    // Importer le modèle dans le graphe
    TF_Status* status = TF_NewStatus();
    TF_ImportGraphDefOptions* importOptions = TF_NewImportGraphDefOptions();
    TF_GraphImportGraphDef(graph, graphDef, importOptions, status);

    // Créer la session
    TF_Session* session = TF_NewSession(graph, options, status);

    // Nettoyage des ressources
    free(data);
    TF_DeleteBuffer(graphDef);
    TF_DeleteImportGraphDefOptions(importOptions);
    TF_DeleteSessionOptions(options);
    TF_DeleteStatus(status);

    return session;
}

// Fonction pour évaluer une liste de listes de troupes
int evaluateNeuralNetwork(TF_Session* session, const char* troops) {
    // Préparation des données d'entrée
    const int numTroups = strlen(troops);
    const int numFeatures = numTroups * numTroups;

    TF_Tensor* inputTensor = TF_AllocateTensor(
        TF_FLOAT,               // Type des données d'entrée
        NULL,                   // Dimensions (NULL pour un tenseur 1D)
        0,                      // Nombre de dimensions
        sizeof(float) * numFeatures  // Taille des données d'entrée
    );

    float* inputData = (float*)TF_TensorData(inputTensor);

    // Convertir la liste de listes de troupes en données d'entrée
    for (int i = 0; i < numTroups; i++) {
        for (int j = 0; j < numTroups; j++) {
            const char troop = troops[i * numTroups + j];
            inputData[i * numTroups + j] = (troop == ' ') ? 0.0f : 1.0f;
        }
    }

    // Préparation du tableau de tenseurs d'entrée
    TF_Output input = { .oper = TF_GraphOperationByName(session->graph, "input"), .index = 0 };
    TF_Tensor* inputTensors[] = { inputTensor };
    TF_Output inputOps[] = { input };
    const int numInputs = 1;

    // Préparation du tableau de tenseurs de sortie
    TF_Output output = { .oper = TF_GraphOperationByName(session->graph, "output"), .index = 0 };
    TF_Tensor* outputTensor = NULL;
    TF_Tensor* outputTensors[] = { &outputTensor };
    TF_Output outputOps[] = { output };
    const int numOutputs = 1;

    // Exécution du réseau de neurones
    TF_Status* status = TF_NewStatus();
    TF_SessionRun(session, NULL, inputOps, inputTensors, numInputs, outputOps, outputTensors, numOutputs, NULL, 0, NULL, status);

    // Récupération du résultat
    const int result = (int)(*((float*)TF_TensorData(outputTensor)));

    // Nettoyage des ressources
    TF_DeleteTensor(inputTensor);
    TF_DeleteStatus(status);

    return result;
}

int main() {
    // Création du réseau de neurones
    TF_Session* session = createNeuralNetwork();

    // Évaluation de la liste de listes de troupes
    const char* troops = "ka ka";
    int result = evaluateNeuralNetwork(session, troops);

    printf("Résultat : %d\n", result);

    // Sauvegarde des poids du modèle
    saveModel(session->graph, session, "path/to/saved_model.pb");

    // Nettoyage des ressources
    TF_DeleteSession(session);

    return 0;
}