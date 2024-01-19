#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define lsize 30 //STRING LENGTH
#define lineSize 300 // SIZE TO STORE EACH LINE OF THE FILE
#define TABLE_SIZE 1000//SIZE OF HASH TABLE

// NEW DATA TYPE (POINTER TO STRUCT)
typedef struct medicine *ptr_medicine;
typedef struct medicine_list *medicine_list_ptr;
typedef struct HashTable  *ptr_hash;

//THE STRUCT OF NODES IN BINARY SEARCH TREE
struct medicine {
    char medicineName[lsize];
    char activeIngredient[lsize];
    char category[lsize];
    char indication[lsize];
    char company[lsize];
    char expiryDate[lsize];
    int counter;
    ptr_medicine left;
    ptr_medicine right;
};

//THE LINKED LIST TO STORE THE MEDICINES
struct medicine_list{
    ptr_medicine medicine;
    struct medicine_list *next;
};
medicine_list_ptr head = NULL;
medicine_list_ptr tail = NULL;

//STRUCT OF HASH NODE
struct HashTable {
        ptr_medicine table[TABLE_SIZE];
        int size;
};

ptr_medicine newMedicine(char medicineName[], char activeIngredient[], char category[], char indication[], char expiryDate[], char company[], int counter);
ptr_medicine insertMedicine(ptr_medicine root, char medicineName[], char activeIngredient[], char category[], char indication[], char expiryDate[], char company[], int counter);
ptr_medicine readFile(ptr_medicine root);
void displayChoices();
void display_menu(ptr_medicine root, struct HashTable *ht);
void displayTree(ptr_medicine root);
ptr_medicine searchMedicine(ptr_medicine root, char medicineName[]);
ptr_medicine addMedicine(ptr_medicine root);
void updateQuantity( ptr_medicine medicine, int newQuantity);
void addToList(ptr_medicine medicine);
void sortedTree(ptr_medicine root);
void listByCategory(ptr_medicine root, char category[]);
void listByIndication(ptr_medicine root, char indication[]);
ptr_medicine deleteMedicine(ptr_medicine root, char medicineName[]);
ptr_medicine minValueNode(ptr_medicine root);
ptr_medicine deleteAsCompany(ptr_medicine root, char company[]);
void inOrderTraversal(ptr_medicine root );//TO VISIT ALL THE NODES
void saveInFile(ptr_medicine root);
void saveInorder(ptr_medicine root, FILE* file);
ptr_hash readHashFile(ptr_hash ht);
ptr_hash createHashTable();
ptr_medicine insertMedicineHash(ptr_medicine node, ptr_medicine m);
void printHashTable(ptr_hash ht);
int hash(char* key);
void addMedicineToHashTable(ptr_hash ht, ptr_medicine medicine);
ptr_medicine searchMedicineInHashTable(ptr_hash ht, char* medicineName);

//FUNCTION TO READ DATA FROM THE FILE
 ptr_medicine readFile(ptr_medicine root) {
    char line[lineSize];
    char* token;
    FILE* medicinesFile = fopen("Meds.txt", "r");
    if (medicinesFile == NULL) {
        printf("Error ! File can not be opened \n");
    }

    while (fgets(line, sizeof(line), medicinesFile)) {
        char medicineName[lsize];
        char activeIngredient[lsize];
        char category[lsize];
        char indication[lsize];
        char expiryDate[lsize];
        int counter;
        char company[lsize];
        token = strtok(line, ":");
        strcpy(medicineName, token);
        token = strtok(NULL, ":");
        strcpy(activeIngredient, token);
        token = strtok(NULL, ":");
        strcpy(category, token);
        token = strtok(NULL, ":");
        strcpy(indication, token);
        token = strtok(NULL, ":");
        strcpy(expiryDate, token);
        token = strtok(NULL, ":");
        counter = atoi(token);
        token = strtok(NULL, ":");
        strcpy(company, token);
        while (token!=NULL){
            token = strtok(NULL, ":");

        }
        root = insertMedicine(root, medicineName, activeIngredient, category, indication, expiryDate, company, counter);
    }

    fclose(medicinesFile);
    displayTree(root);
    return root;
}

// FUNCTION TO CREATE A NEW NODE
ptr_medicine newMedicine(char medicineName[], char activeIngredient[], char category[], char indication[], char expiryDate[], char company[], int counter) {

    ptr_medicine newNode = (ptr_medicine)malloc(sizeof(struct medicine));
    strcpy(newNode->medicineName, medicineName);
    strcpy(newNode->activeIngredient, activeIngredient);
    strcpy(newNode->category, category);
    strcpy(newNode->indication, indication);
    strcpy(newNode->expiryDate, expiryDate);
    strcpy(newNode->company, company);
    newNode->counter = counter;
    newNode->left = newNode->right = NULL;
    return newNode;

}

// FUNCTION TO INSERT A NEW NODE IN THE BST
ptr_medicine insertMedicine(ptr_medicine root, char medicineName[], char activeIngredient[], char category[], char indication[], char expiryDate[], char company[], int counter) {

    if (root == NULL) {
        return newMedicine(medicineName, activeIngredient, category, indication, expiryDate, company, counter);
    }
    if (strcmp(medicineName, root->medicineName) < 0) {
        root->left = insertMedicine(root->left, medicineName, activeIngredient, category, indication, expiryDate, company, counter);
    } else if (strcmp(medicineName, root->medicineName) > 0) {
        root->right = insertMedicine(root->right, medicineName, activeIngredient, category, indication, expiryDate, company, counter);
    }
    return root;

}

//FUNCTION TO DISPLAY MENU CHOICES
void displayChoices() {
    printf("1. Load the medicine information file\n");
    printf("2. Add a new medicine\n");
    printf("3. Search about specific medicine\n");
    printf("4. Update the quantity of a specific medicine\n");
    printf("5. List the medicines in alphabetical order\n");
    printf("6. List all medicines of same category\n");
    printf("7. List all medicines that treat a specific disease\n");
    printf("8. Delete a medicine\n");
    printf("9. Delete all medicines from a specific company\n");
    printf("10. Save all information to a file\n");
    printf("11. Print hashed table\n");
    printf("12. Print the size of hash table\n");
    printf("13. Print the used hash function\n");
    printf("14. Insert a new record into the hash table\n");
    printf("15. Search for a specific medicine in the hash table\n");
    printf("16. Delete a specific record\n");
    printf("17. Save the data back to the file\n");
    printf("18. Exit\n");
}

int numberOfReadings = 0;// TO LET THE USER READ FROM THE FILE ONLY ONE TIME AND MUST READ FIRST

//FUNCTION TO DO WHAT THE USER CHOCE
void display_menu(ptr_medicine root, struct HashTable *ht) {
    int choice;
    char medicineName[lsize];
    char activeIngredient[lsize];
    char category[lsize];
    char indication[lsize];
    char company[lsize];
    char expiryDate[lsize];
    int counter;
    printf("\n------------------------------ MENU ---------------------------------\n");
    displayChoices();
    printf("Enter your choice:\n"); //TAKE FROM THE USER ANY OPTION HE WANTS
    while (1) {
        scanf("%d", &choice);
        switch (choice) {
            case 1: //LOAD THE DATA
                if(numberOfReadings == 0 ){
                    numberOfReadings++;
                    root=readFile(root);
                    printf("The file has been read successfully\n");
                    display_menu(root,ht);
                }else{
                    printf("You had chosen it before!\n");
                    display_menu(root,ht);
        }
               break;
            case 2: // ADD NEW MEDICINE
                if(numberOfReadings != 0){
                root=addMedicine(root);
                displayTree(root);
                display_menu(root,ht);
                }
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 3: // FIND A MEDICINE
                 if(numberOfReadings != 0){
                printf("Enter the name of the medicine you want to search for: ");
                scanf("%s", medicineName);
                ptr_medicine search = searchMedicine(root, medicineName);
                if (search == NULL) {
                    printf("Medicine not found\n");
                } else {
                    displayTree(search);
                }
                     display_menu(root,ht);}
                    else{
                        printf("You should read the data first!!\n");
                         display_menu(root,ht);
                }

                break;
            case 4: // UPDATE THE QUANTITY OF A MEDICINE
                if(numberOfReadings != 0){
                printf("Enter the name of the medicine you want to update its quantity: ");
                scanf("%s", medicineName);
                printf("Enter the new quantity:");
                scanf("%d",&counter);
                ptr_medicine update = searchMedicine(root, medicineName);
                if (update == NULL) {
                    printf("Medicine not found\n");
                } else {
                   updateQuantity(update,counter);
                }
                display_menu(root,ht);}
                 else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 5: //LIST THE MEDICINES IN ALPHABETICAL ORDER
                if(numberOfReadings != 0){
                sortedTree(root);
                display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 6: //LIST ALL MEDICINES OF SAME CATEGORY
                if(numberOfReadings != 0){
                printf("Enter the category that you want to sort by: ");
                fflush(stdin);
                scanf("%s",category);
                listByCategory(root,category);
                display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                 break;
            case 7: //LIST ALL MEDICINES THAT TREAT A SPECIFIC DISEASE
                if(numberOfReadings != 0){
                printf("Enter the name of the disease:");
                scanf("%s", indication);
                listByIndication(root,indication);
                display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 8: //DELETE A MEDICINE
                if(numberOfReadings != 0){
                printf("Enter the name of the medicine you want to delete: ");
                scanf("%s", medicineName);
                root = deleteMedicine(root,medicineName);
                display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 9: //DELETE ALL MEDICINES FROM A SPECIFIC COMPANY
                if(numberOfReadings != 0){
                    printf("Enter the name of the company you want to delete its medicines: ");
                    scanf("%s", company);
                    root = deleteAsCompany(root,company);
                    display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 10: //SAVE ALL INFORMATION TO A FILE
                if(numberOfReadings != 0){
                    saveInFile(root);
                    display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 11: //PRINT HASHED TABLE
                if(numberOfReadings != 0){
                    ht = readHashFile(ht);
                    display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 12: //PRINT THE SIZE OF HASH TABLE
                if(numberOfReadings != 0){
                    display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 13: //PRINT THE USED HASH FUNCTION
                if(numberOfReadings != 0){
                    display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 14: //INSERT A NEW RECORD INTO THE HASH TABLE
                if(numberOfReadings != 0){
                    display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 15: //SEARCH FOR A SPECIFIC MEDICINE IN THE HASH TABLE
                if(numberOfReadings != 0){
                    printf("Enter the name of the medicine you want to search for: ");
                    scanf("%s", medicineName);
                    ptr_medicine search = searchMedicineInHashTable(ht, medicineName);
                    if (search == NULL) {
                        printf("Medicine not found\n");
                    } else {
                        printHashTable(search);}
                    display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 16: //DELETE A SPECIFIC RECORD
                if(numberOfReadings != 0){
                    display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 17: //SAVE THE DATA BACK TO THE FILE
                if(numberOfReadings != 0){
                    display_menu(root,ht);}
                else{
                        printf("You should read the data first!!\n");
                        display_menu(root,ht);
                }
                break;
            case 18: //Exit
                exit(0);
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
    }
}

void displayTree(ptr_medicine root) {

    if (root != NULL) {

        displayTree(root->left);
        printf("Medicine Name: %s\n", root->medicineName);
        printf("Active Ingredient: %s\n", root->activeIngredient);
        printf("Category: %s\n", root->category);
        printf("Indication: %s\n", root->indication);
        printf("Company: %s\n", root->company);
        printf("Expiry Date: %s\n", root->expiryDate);
        printf("Counter: %d\n", root->counter);
        printf("\n");
        displayTree(root->right);
    }

}

ptr_medicine addMedicine(ptr_medicine root) { //ADD NEW MEDICINE FROM THE USER

    char medicineName[lsize];
    char activeIngredient[lsize];
    char category[lsize];
    char indication[lsize];
    char company[lsize];
    char expiryDate[lsize];
    int counter; //QUANTITY

    printf("Enter the name of the medicine: ");
    scanf("%s", medicineName);
    printf("Enter the active ingredient of the medicine: ");
    scanf("%s", activeIngredient);
    printf("Enter the category of the medicine: ");
    scanf("%s", category);
    printf("Enter the indication of the medicine: ");
    scanf("%s", indication);
    printf("Enter the company of the medicine: ");
    scanf("%s", company);
    printf("Enter the expiry date of the medicine: ");
    scanf("%s", expiryDate);
    printf("Enter the counter of the medicine: ");
    scanf("%d", &counter);
    root = insertMedicine(root, medicineName, activeIngredient, category, indication, expiryDate, company, counter);
    return root;

}

ptr_medicine searchMedicine(ptr_medicine root, char medicineName[]) { //TO SEARCH FOR THE MEDICINE WITH THE GIVEN NAME

    if (root == NULL) {
        return NULL;
    }
    int compare = strcmp(medicineName, root->medicineName);
    if (compare < 0) {
        return searchMedicine(root->left, medicineName);
    } else if (compare > 0) {
        return searchMedicine(root->right, medicineName);
    } else {
        return root;
    }

}

void updateQuantity( ptr_medicine medicine, int newQuantity) {

  medicine->counter = newQuantity;

}

void addToList(ptr_medicine medicine){ //FUNCTION TO ADD THE MEDICINE TO THE LINKED LIST

    medicine_list_ptr newNode = (medicine_list_ptr)malloc(sizeof(struct medicine_list));
    newNode->medicine = medicine;
    newNode->next = NULL;
    if(head == NULL){
        head = tail = newNode;
    }
    else{
        tail->next = newNode;
        tail = newNode;
    }

}

void sortedTree(ptr_medicine root) { //TO LIST THE MEDICINES IN ALPHABETICAL

    if (root == NULL) return;
    inOrderTraversal(root);
    medicine_list_ptr curr = head;
    while(curr != NULL){
        printf("Name: %s\n Active Ingredient: %s\n Category: %s\n Indication: %s\n Expiry Date: %s\n Company: %s\n Quantity: %d\n", curr->medicine->medicineName, curr->medicine->activeIngredient, curr->medicine->category, curr->medicine->indication, curr->medicine->expiryDate, curr->medicine->company, curr->medicine->counter);
        curr = curr->next;
    }

}

void inOrderTraversal(ptr_medicine root){ //TO VISIT ALL THE NODES

    if(root != NULL){
        inOrderTraversal(root->left);
        addToList(root);
        inOrderTraversal(root->right);
    }

}

void listByCategory(ptr_medicine root, char category[]) {
    if (root == NULL) return;
    listByCategory(root->left, category);
    if(strlen(category) > 1 && strcmp(root->category,category) == 0) {
        printf("Name: %s\n Active Ingredient: %s\n Category: %s\n Indication: %s\n Expiry Date: %s\n Company: %s\n Quantity: %d\n", root->medicineName, root->activeIngredient, root->category, root->indication, root->expiryDate, root->company, root->counter);
    }
    listByCategory(root->right, category);
}

void listByIndication(ptr_medicine temp, char indication[]) {
    if(temp == NULL)
        return;
    char *token= strtok(indication," ");
    while(token != NULL){
        if(strstr(temp->indication, token) != NULL) {
            printf("Name: %s\n Active Ingredient: %s\n Category: %s\n Indication: %s\n Expiry Date: %s\n Company: %s\n Quantity: %d\n", temp->medicineName, temp->activeIngredient, temp->category, temp->indication, temp->expiryDate, temp->company, temp->counter);
            break;
        }
        token = strtok(NULL, " ");
    }
    listByIndication(temp->left, indication);
    listByIndication(temp->right, indication);
}

ptr_medicine deleteMedicine(ptr_medicine root, char medicineName[]) {
    if (root == NULL) {
        return root;
    }
    if (strcmp(medicineName, root->medicineName) < 0) {
        root->left = deleteMedicine(root->left, medicineName);
    } else if (strcmp(medicineName, root->medicineName) > 0) {
        root->right = deleteMedicine(root->right, medicineName);
    } else {
        if (root->left == NULL) {
            ptr_medicine temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            ptr_medicine temp = root->left;
            free(root);
            return temp;
        }
        ptr_medicine temp = minValueNode(root->right);
        strcpy(root->medicineName,temp->medicineName);
        root->right = deleteMedicine(root->right, temp->medicineName);
    }
    return root;
}

ptr_medicine minValueNode(ptr_medicine root) {

    ptr_medicine current = root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;

}

ptr_medicine deleteAsCompany(ptr_medicine root, char company[]) {

    if (root == NULL) {
        printf("No medicines from the specified company found\n");
        return NULL;
    }
    if (strcmp(root->medicineName, company) < 0) {
        root->left = deleteAsCompany(root->left, company);
    } else if (strcmp(root->medicineName, company) > 0) {
        root->right = deleteAsCompany(root->right, company);
    } else {
        if (strcmp(root->company, company) == 0) {
            printf("Deleting medicine %s\n", root->medicineName);
            root = deleteMedicine(root, root->medicineName);
        }
        root->left = deleteAsCompany(root->left, company);
        root->right = deleteAsCompany(root->right, company);
    }
    if (root != NULL) {
        printf("Successfully deleted all medicines from %s\n", company);
    }
    return root;
}

void saveInFile(ptr_medicine root) {
    FILE* medicinesFile = fopen("Pharmacy.txt", "w");
    if (medicinesFile == NULL) {
        printf("Error ! File can not be opened \n");
        return;
    }
    saveInorder(root, medicinesFile);
    fclose(medicinesFile);
    printf("Successfully saved all information to Pharmacy.txt\n");
}

void saveInorder(ptr_medicine root, FILE* file) {
    if (root == NULL) {
        return;
    }
    saveInorder(root->left, file);
    fprintf(file, "%s:%s:%s:%s:%s:%d:%s\n", root->medicineName, root->activeIngredient, root->category, root->indication, root->expiryDate, root->counter, root->company);
    saveInorder(root->right, file);
}

 ptr_hash readHashFile (ptr_hash ht){

        FILE *file = fopen("Pharmacy.txt", "r");
        ht = createHashTable();
        if (file == NULL) {
            printf("Error opening file!\n");
        }
        while (!feof(file)) {
            ptr_medicine m;
            fscanf(file, "%s:%s:%s:%s:%s:%d:%s", m->medicineName, m->activeIngredient, m->category, m->indication, m->expiryDate, m->counter, m->company);
            addMedicineToHashTable(ht, m);
        }
        fclose(file);
        printHashTable(ht);
        return ht;

}

// Function to create a new Hash Table
ptr_hash createHashTable(){
    ptr_hash ht = (ptr_hash) malloc(sizeof(struct HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
    return ht;
}
// Function to calculate the hash value for a given key
int hash(char* key) {
    int sum = 0;
    for (int i = 0; i < strlen(key); i++) {
        sum += key[i];
    }
    return sum % TABLE_SIZE;
}

// Function to add a new medicine to the Hash Table
void addMedicineToHashTable(ptr_hash ht, ptr_medicine medicine) {
    unsigned int index = hash(medicine->medicineName);
    ht->table[index] = insertMedicineHash(ht->table[index], medicine);
    ht->size++;
}

ptr_medicine insertMedicineHash(ptr_medicine node, ptr_medicine m) {
    if (node == NULL) {
        node = (struct medicine*) malloc(sizeof(struct medicine));
        strcpy(node->medicineName, m->medicineName);
        strcpy(node->activeIngredient, m->activeIngredient);
        strcpy(node->category, m->category);
        strcpy(node->indication, m->indication);
        strcpy(node->expiryDate, m->expiryDate);
        node->counter = m->counter;
        strcpy(node->company, m->company);
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    if (strcmp(m->medicineName, node->medicineName) < 0) {
        node->left = insertMedicineHash(node->left,m);
    } else if (strcmp(m->medicineName, node->medicineName) > 0) {
        node->right = insertMedicineHash(node->medicineName,m);
    }
    return node;
}


// Function to search for a medicine in the Hash Table
ptr_medicine searchMedicineInHashTable(ptr_hash ht, char* medicineName) {
    unsigned int index = hash(medicineName);
    ptr_medicine node = ht->table[index];
    while (node != NULL) {
        if (strcmp(node->medicineName, medicineName) == 0) {
            return node;
        }
        if (strcmp(medicineName, node->medicineName) < 0) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return node;
}
// Function to print the Hash Table
void printHashTable(ptr_hash ht) {

    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Index %d: ", i);
        if (ht->table[i] != NULL) {
            displayTree(ht->table[i]);
        } else {
            printf("(empty)\n");
        }
    }

}
int main() {

    ptr_medicine root = (ptr_medicine)malloc(sizeof(struct medicine));
    root = NULL ;
    ptr_hash ht = (ptr_hash)malloc(sizeof(struct HashTable ));
    display_menu(root,ht); // DISPLAY THE OPTIONS TO THE USER
    return 0;

}
