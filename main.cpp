#include <vector>
#include <string>

#include <stdio.h>
#include <array>
#include <complex>
#include <iostream>
 
// Nonzero value enables csv logging.
#define LOG_TO_CSV_NUMSAMPLES() 50
 
// ===== Example 1 - One Neuron, One training Example =====
 
void Example1RunNetwork (
    float input, float desiredOutput,
    float weight, float bias,
    float& error, float& cost, float& actualOutput,
    float& deltaCost_deltaWeight, float& deltaCost_deltaBias, float& deltaCost_deltaInput
) {
    // calculate Z (weighted input) and O (activation function of weighted input) for the neuron
    float Z = input * weight + bias;
    float O = 1.0f / (1.0f + std::exp(-Z));
 
    // the actual output of the network is the activation of the neuron
    actualOutput = O;
 
    // calculate error
    error = std::abs(desiredOutput - actualOutput);
 
    // calculate cost
    cost = 0.5f * error * error;
 
    // calculate how much a change in neuron activation affects the cost function
    // deltaCost/deltaO = O - target
    float deltaCost_deltaO = O - desiredOutput;
 
    // calculate how much a change in neuron weighted input affects neuron activation
    // deltaO/deltaZ = O * (1 - O)
    float deltaO_deltaZ = O * (1 - O);
 
    // calculate how much a change in a neuron's weighted input affects the cost function.
    // This is deltaCost/deltaZ, which equals deltaCost/deltaO * deltaO/deltaZ
    // This is also deltaCost/deltaBias and is also refered to as the error of the neuron
    float neuronError = deltaCost_deltaO * deltaO_deltaZ;
    deltaCost_deltaBias = neuronError;
 
    // calculate how much a change in the weight affects the cost function.
    // deltaCost/deltaWeight = deltaCost/deltaO * deltaO/deltaZ * deltaZ/deltaWeight
    // deltaCost/deltaWeight = neuronError * deltaZ/deltaWeight
    // deltaCost/deltaWeight = neuronError * input
    deltaCost_deltaWeight = neuronError * input;
 
 
    // As a bonus, calculate how much a change in the input affects the cost function.
    // Follows same logic as deltaCost/deltaWeight, but deltaZ/deltaInput is the weight.
    // deltaCost/deltaInput = neuronError * weight
    deltaCost_deltaInput = neuronError * weight;
}
 
void Example1 ()
{
    #if LOG_TO_CSV_NUMSAMPLES() > 0
        // open the csv file for this example
        FILE *file = fopen("Example1.csv","w+t");
        if (file != nullptr)
            fprintf(file, "trainingIndex error cost weight bias dCost/dWeight dCost/dBias dCost/dInput \n");
    #endif
 
    // learning parameters for the network
    const float c_learningRate = 0.5f;
    const size_t c_numTrainings = 10000;
 
    // training data
    // input: 1, output: 0
    const std::array<float, 2> c_trainingData = {1.0f, 0.0f};
 
    // starting weight and bias values
    float weight = 0.3f;
    float bias = 0.5f;
 
    // iteratively train the network
    float error = 0.0f;
    for (size_t trainingIndex = 0; trainingIndex < c_numTrainings; ++trainingIndex)
    {
        // run the network to get error and derivatives
        float output = 0.0f;
        float cost = 0.0f;
        float deltaCost_deltaWeight = 0.0f;
        float deltaCost_deltaBias = 0.0f;
        float deltaCost_deltaInput = 0.0f;
        Example1RunNetwork(c_trainingData[0], c_trainingData[1], weight, bias, error, cost, output, deltaCost_deltaWeight, deltaCost_deltaBias, deltaCost_deltaInput);
 
        #if LOG_TO_CSV_NUMSAMPLES() > 0
            const size_t trainingInterval = (c_numTrainings / (LOG_TO_CSV_NUMSAMPLES() - 1));
            if (file != nullptr && (trainingIndex % trainingInterval == 0 || trainingIndex == c_numTrainings - 1))
            {
                // log to the csv
                fprintf(file, "%zi %f %f %f %f %f %f %f \n", trainingIndex, error, cost, weight, bias, deltaCost_deltaWeight, deltaCost_deltaBias, deltaCost_deltaInput);
            }
        #endif
 
        // adjust weights and biases
        weight -= deltaCost_deltaWeight * c_learningRate;
        bias -= deltaCost_deltaBias * c_learningRate;
    }
 
    printf("Example1 Final Error: %f \n", error);
 
    #if LOG_TO_CSV_NUMSAMPLES() > 0
        if (file != nullptr)
            fclose(file);
    #endif
}

int main()
{
//    std::vector<std::string> msg {"This","is","autograd", "in", "C++"};
    
//     for (const std::string& word : msg)
//     {
//         std::cout << word << " ";
//     }
//     std::cout << std::endl;

    Example1();
 }