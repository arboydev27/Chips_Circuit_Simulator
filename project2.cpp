/*
 * Chip Circuit Simulation Program
 * Author: Arboy Magomba
 * Date: September 27, 2021
 * Description: This program simulates the behavior of different types of chips in a circuit.
 * Each chip can perform a specific operation such as addition, subtraction, multiplication,
 * division or negation based on its type. The chips can be connected to each other and the result
 * is computed and displayed.
 */

#include <iostream>
#include <string>
using namespace std;

class Chip {
private:
    char chipType;        // Type of the chip (A: Addition, S: Subtraction, etc.)
    string id;            // Unique identifier for the chip
    Chip* input1;         // Pointer to the first input chip
    Chip* input2;         // Pointer to the second input chip (optional, can be NULL)
    Chip* output;         // Pointer to the output chip (NULL for output chips)
    double inputValue;    // Input value for input chips (used in I type chips)
    double result;        // Computed result for non-input chips

public:
    // Constructor to initialize the chip with a type and unique ID
    Chip(char type, const string& id);

    // Sets the first input chip for this chip
    void setInput1(Chip* inputChip);

    // Sets the second input chip (used for operations requiring two inputs)
    void setInput2(Chip* inputChip);

    // Sets the output chip that this chip connects to
    void setOutput(Chip* outputChip);

    // Performs the operation based on the chip type
    void compute();

    // Displays the chip's details (inputs, outputs, ID)
    void display() const;

    // Returns the chip's ID
    string getId() const;

    // Returns the chip's type (A, S, M, D, etc.)
    char getChipType() const;

    // Returns the result computed by this chip
    double getResult() const;

    // Sets the input value for input chips (used for testing input chips)
    void setInputValue(double value) {
        inputValue = value;
    }

    // Returns the first input chip (for internal logic and testing)
    Chip* getInput1() const {
        return input1;
    }
};

// Constructor Implementation
Chip::Chip(char type, const string& id) {
    chipType = type;             // Set chip type (e.g., A, S, M)
    this->id = id;               // Set the chip ID
    input1 = nullptr;            // Initialize input1 as null
    input2 = nullptr;            // Initialize input2 as null (optional)
    output = nullptr;            // Output is initially null (set later)
}

void Chip::setInput1(Chip* inputChip) {
    input1 = inputChip;          // Connect input1 to this chip
    inputChip->setOutput(this);  // Set this chip as the output of inputChip
}

void Chip::setInput2(Chip* inputChip) {
    input2 = inputChip;          // Connect input2 to this chip
    inputChip->setOutput(this);  // Set this chip as the output of inputChip
}

void Chip::setOutput(Chip* outputChip) {
    output = outputChip;         // Connect this chip to outputChip
}

// Perform the operation based on the chip type
void Chip::compute() {
    // If it's an input chip, directly return its value
    if (chipType == 'I') {
        result = inputValue;     // Input chip simply passes its value
        return;
    }

    // Compute the results of the input chips before performing this chip's operation
    if (input1) input1->compute();   // Ensure input1 is computed
    if (input2) input2->compute();   // Ensure input2 is computed if it exists

    // Perform operation based on the chip type
    if (chipType == 'A') {            // Addition chip
        result = input1->getResult() + input2->getResult();
    }
    else if (chipType == 'S') {       // Subtraction chip
        result = input1->getResult() - input2->getResult();
    }
    else if (chipType == 'M') {       // Multiplication chip
        result = input1->getResult() * input2->getResult();
    }
    else if (chipType == 'D') {       // Division chip
        if (input2->getResult() != 0) {  // Handle division by zero
            result = input1->getResult() / input2->getResult();
        } else {
            cout << "Error: Division by zero in chip " << id << endl;
            result = 0;
        }
    }
    else if (chipType == 'N') {  // Negation chip
        if (input1->getResult() != 0) {
            result = -input1->getResult();
         }
    }
}

// Displays the chip's connections and output
void Chip::display() const {
    if (chipType == 'I') {  // Display for input chip
        cout << id << ", Output = " << (output ? output->getId() : "None") << endl;
    }
    else if (chipType == 'O') {  // Display for output chip
        cout << id << ", Input 1 = " << (input1 ? input1->getId() : "None") << endl;
    }
    else {  // Display for other chips with two inputs and an output
        cout << id << ", Input 1 = " << (input1 ? input1->getId() : "None");
        cout << ", Input 2 = " << (input2 ? input2->getId() : "None");
        cout << ", Output = " << output->getId();
        cout << endl;
    }
}

string Chip::getId() const {
    return id;  // Return the chip's unique ID
}

char Chip::getChipType() const {
    return chipType;  // Return the chip type (A, S, M, etc.)
}

double Chip::getResult() const {
    return result;  // Return the result of the chip's operation
}

// Main function
int main(){
    // Step 1: Read the number of Chips from input
    int numChips;
    cin >> numChips;

    // Step 2: Create an array of Chip pointers
    Chip** allChips = new Chip*[numChips];

    // Step 3: Initialize the chips by reading their IDs and creating Chip objects
    for(int i = 0; i < numChips; i++){
        string chipId;
        cin >> chipId;

        char type = chipId[0];           // Determine the chip type based on the first character
        allChips[i] = new Chip(type, chipId);  // Create and store the Chip object
    }

    // Step 4: Read the number of commands to process
    int numCommands;
    cin >> numCommands;

    // Step 5: Process each command
    for(int i = 0; i < numCommands; i++){
        string command;
        cin >> command;

        if (command == "A") {   // If command is to add a connection between chips
            string inputId, outputId;
            cin >> inputId >> outputId;

            Chip* inputChip = nullptr;
            Chip* outputChip = nullptr;

            // Find the input and output chips in the array
            for(int j = 0; j < numChips; j++){
                if(allChips[j]->getId() == inputId){
                    inputChip = allChips[j];
                }
                if(allChips[j]->getId() == outputId){
                    outputChip = allChips[j];
                }
            }

            // Make appropriate connections based on the chip types
            if (outputChip->getChipType() == 'N' || outputChip->getChipType() == 'O') {
                outputChip->setInput1(inputChip);  // Negation/output chips take only one input
            }
            else if (outputChip->getChipType() == 'A' || outputChip->getChipType() == 'S' || outputChip->getChipType() == 'M' || outputChip->getChipType() == 'D') {
                if (outputChip->getInput1() == nullptr) {
                    outputChip->setInput1(inputChip);
                }
                else {
                    outputChip->setInput2(inputChip);
                }
            }
        }
        else if (command == "I") {   // If command is to set input values for input chips
            string chipId;
            double value;
            cin >> chipId >> value;

            // Find the input chip and set its value
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == chipId) {
                    allChips[j]->setInputValue(value);
                    break;
                }
            }
        }
        else if (command == "O") {   // If command is to output the result of a chip
            string outputChipId;
            cin >> outputChipId;

            // Find the output chip, compute its result, and display it
            cout << "Computation Starts " << endl;
            for (int j = 0; j < numChips; j++) {
                if (allChips[j]->getId() == outputChipId) {
                    if (allChips[j]->getChipType() == 'O') {
                        allChips[j]->compute();  // Compute the result
                        cout << "The output value from this circuit is " << allChips[j]->getInput1()->getResult() << endl;
                    } else {
                        allChips[j]->compute();
                        cout << "The output value from this circuit is " << allChips[j]->getResult() << endl;
                    }
                    break;
                }
            }
        }
    }

    // Step 6: Display the connections that were established
    cout << "***** Showing the connections that were established" << endl;
    for(int i = 0; i < numChips; i++){
        if (allChips[i]->getId() != "O50") { // Skip output chip during this pass
            allChips[i]->display();
        }
    }
    for (int i = 0; i < numChips; ++i) {
        if (allChips[i]->getChipType() == 'O') {
            allChips[i]->display();   // Show output connections last
        }
    }

    // Clean up dynamically allocated memory
    for(int i = 0; i < numChips; i++){
        delete allChips[i];  // Free each chip object
    }
    delete[] allChips;       // Free the array of chip pointers

    return 0;
}
