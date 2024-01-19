#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define lsize 30 //STRING LENGTH
#define lineSize 300 // SIZE TO STORE EACH LINE OF THE FILE
#define TABLE_SIZE 7//SIZE OF HASH TABLE
#define LoadFactor 0.75
int SIZEHASH=7;
// NEW DATA TYPE (POINTER TO STRUCT)
typedef struct medicine *ptr_medicine;
typedef struct medicine_list *medicine_list_ptr;
typedef struct HashTable  *ptr_hash;

//THE STRUCT OF NODES IN BINARY SEARCH TREE
struct medicine
{
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
struct medicine_list
{
    ptr_medicine medicine;
    struct medicine_list *next;
};
medicine_list_ptr head = NULL;
medicine_list_ptr tail = NULL;

//STRUCT OF HASH NODE
struct HashTable
{
    ptr_medicine table[TABLE_SIZE];
    int size;
};
ptr_hash ht;
ptr_medicine newMedicine(char medicineName[], char activeIngredient[], char category[], char indication[], char expiryDate[], char company[], int counter);
ptr_medicine insertMedicine(ptr_medicine root, char medicineName[], char activeIngredient[], char category[], char indication[], char expiryDate[], char company[], int counter);
ptr_medicine readFile(ptr_medicine root);
void displayChoices();
void ReHash();

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
int hash(char *medicineName);
void addMedicineHash(ptr_hash ht, ptr_medicine med);
ptr_hash readHashFile(ptr_hash ht);
void printHashTable(ptr_hash ht);
ptr_medicine searchMedicineInHashTable(ptr_hash ht, char* medicineName);
void printHashFunction(ptr_hash ht);
void addRecord(ptr_hash ht);
ptr_hash deleteMedicineHash(ptr_hash ht, char* medicineName);
void saveHashToFile(ptr_hash ht);

//FUNCTION TO READ DATA FROM THE FILE
ptr_medicine readFile(ptr_medicine root)
{
    char line[lineSize];
    char* token;
    FILE* medicinesFile = fopen("Meds.txt", "r");
    if (medicinesFile == NULL)
    {
        printf("Error ! File can not be opened \n");
    }

    while (fgets(line, sizeof(line), medicinesFile))
    {
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
        while (token!=NULL)
        {
            token = strtok(NULL, ":");

        }
        if (company[strlen(company)-1]=='\n')
        {
            company[strlen(company) - 1] = '\0';
        }
        root = insertMedicine(root, medicineName, activeIngredient, category, indication, expiryDate, company, counter);
    }

    fclose(medicinesFile);
    displayTree(root);
    return root;
}

// FUNCTION TO CREATE A NEW NODE
ptr_medicine newMedicine(char medicineName[], char activeIngredient[], char category[], char indication[], char expiryDate[], char company[], int counter)
{

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
ptr_medicine insertMedicine(ptr_medicine root, char medicineName[], char activeIngredient[], char category[], char indication[], char expiryDate[], char company[], int counter)
{

    if (root == NULL)
    {
        return newMedicine(medicineName, activeIngredient, category, indication, expiryDate, company, counter);
    }
    if (strcmp(medicineName, root->medicineName) < 0)
    {
        root->left = insertMedicine(root->left, medicineName, activeIngredient, category, indication, expiryDate, company, counter);
    }
    else if (strcmp(medicineName, root->medicineName) > 0)
    {
        root->right = insertMedicine(root->right, medicineName, activeIngredient, category, indication, expiryDate, company, counter);
    }
    return root;

}

//FUNCTION TO DISPLAY MENU CHOICES
void displayChoices()
{
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
void display_menu(ptr_medicine root, struct HashTable *ht)
{
    int choice=1;
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
    scanf("%d", &choice);
    while (1)
    {
        switch (choice)
        {
            case 1: //LOAD THE DATA
                if(numberOfReadings == 0 )
                {
                    numberOfReadings++;
                    root=readFile(root);
                    printf("The file has been read successfully\n");
                }
                else
                {
                    printf("You had chosen it before!\n");
                }
                break;
            case 2: // ADD NEW MEDICINE
                if(numberOfReadings != 0)
                {
                    root=addMedicine(root);
                    displayTree(root);
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 3: // FIND A MEDICINE
                if(numberOfReadings != 0)
                {
                    printf("Enter the name of the medicine you want to search for: ");
                    scanf("%s", medicineName);
                    ptr_medicine search = searchMedicine(root, medicineName);
                    if (search == NULL)
                    {
                        printf("Medicine not found\n");
                    }
                    else
                    {
                        displayTree(search);
                    }
                }
                else
                {
                    printf("You should read the data first!!\n");
                }

                break;
            case 4: // UPDATE THE QUANTITY OF A MEDICINE
                if(numberOfReadings != 0)
                {
                    printf("Enter the name of the medicine you want to update its quantity: ");
                    scanf("%s", medicineName);
                    printf("Enter the new quantity:");
                    scanf("%d",&counter);
                    ptr_medicine update = searchMedicine(root, medicineName);
                    if (update == NULL)
                    {
                        printf("Medicine not found\n");
                    }
                    else
                    {
                        updateQuantity(update,counter);
                    }
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 5: //LIST THE MEDICINES IN ALPHABETICAL ORDER
                if(numberOfReadings != 0)
                {
                    sortedTree(root);
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 6: //LIST ALL MEDICINES OF SAME CATEGORY
                if(numberOfReadings != 0)
                {
                    printf("Enter the category that you want to sort by: ");
                    strcpy(category,"");
                    while(strcmp(category,"")==0)
                    {
                        gets(category);
                    }
                    listByCategory(root, category);
                    break;
                }
                else
                {
                    printf("You should read the data first!!\n");

                    break;
                }
            case 7: //LIST ALL MEDICINES THAT TREAT A SPECIFIC DISEASE
                if(numberOfReadings != 0)
                {
                    printf("Enter the name of the disease:");
                    strcpy(indication,"");
                    while(strcmp(indication,"")==0)
                    {
                        gets(indication);
                    }
                    listByIndication(root,indication);
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 8: //DELETE A MEDICINE
                if(numberOfReadings != 0)
                {
                    printf("Enter the name of the medicine you want to delete: ");
                    strcpy(medicineName,"");
                    while(strcmp(medicineName,"")==0)
                    {
                        gets(medicineName);
                    }
                    root = deleteMedicine(root, medicineName);
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 9: //DELETE ALL MEDICINES FROM A SPECIFIC COMPANY
                if(numberOfReadings != 0)
                {
                    printf("Enter the name of the company you want to delete its medicines: ");
                    strcpy(company,"");
                    while(strcmp(company,"")==0)
                    {
                        gets(company);
                    }
                    root = deleteAsCompany(root,company);
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 10: //SAVE ALL INFORMATION TO A FILE
                if(numberOfReadings != 0)
                {
                    saveInFile(root);
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 11: //PRINT HASHED TABLE
                if(numberOfReadings != 0)
                {
                    ht=readHashFile(ht);
                    printHashTable(ht);

                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 12: //PRINT THE SIZE OF HASH TABLE
                if(numberOfReadings != 0)
                {
                    printf("Table size : %d\n", ht->size);
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 13: //PRINT THE USED HASH FUNCTION
                if(numberOfReadings != 0)
                {
                    printHashFunction(ht);
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 14: //INSERT A NEW RECORD INTO THE HASH TABLE
                if(numberOfReadings != 0)
                {
                    addRecord(ht);
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 15: //SEARCH FOR A SPECIFIC MEDICINE IN THE HASH TABLE
                if(numberOfReadings != 0)
                {
                    printf("Enter the name of the medicine you want to search for: ");
                    scanf("%s", medicineName);
                    ptr_medicine search = searchMedicineInHashTable(ht, medicineName);
                    if (search == NULL)
                    {
                        printf("Medicine not found\n");
                    }
                    else
                    {
                        printHashTable(search);
                    }
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 16: //DELETE A SPECIFIC RECORD
                if(numberOfReadings != 0)
                {
                    printf("Enter the name of the medicine you want to delete: ");
                    strcpy(medicineName,"");
                    while(strcmp(medicineName,"")==0)
                    {
                        gets(medicineName);
                    }
                    ht = deleteMedicineHash(ht, medicineName);
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 17: //SAVE THE DATA BACK TO THE FILE
                if(numberOfReadings != 0)
                {
                    saveHashToFile(ht);
                }
                else
                {
                    printf("You should read the data first!!\n");
                }
                break;
            case 18: //Exit
                exit(0);
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
        printf("\n------------------------------ MENU ---------------------------------\n");
        displayChoices();
        printf("Enter your choice:\n"); //TAKE FROM THE USER ANY OPTION HE WANTS
        scanf("%d", &choice);
    }
}

//TO PRINT THE BST
void displayTree(ptr_medicine root)
{

    if (root != NULL)
    {

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

//INSERT MEDICINE FROM THE USER
ptr_medicine addMedicine(ptr_medicine root)
{
    char medicineName[lsize];
    char activeIngredient[lsize];
    char category[lsize];
    char indication[lsize];
    char company[lsize];
    char expiryDate[lsize];
    int counter; //QUANTITY

    printf("Enter the name of the medicine: ");
    fflush(stdin);
    fgets(medicineName, lsize, stdin);
    medicineName[strcspn(medicineName, "\n")] = 0;

    printf("Enter the active ingredient of the medicine: ");
    fflush(stdin);
    fgets(activeIngredient, lsize, stdin);
    activeIngredient[strcspn(activeIngredient, "\n")] = 0;


    printf("Enter the category of the medicine: ");
    fflush(stdin);
    fgets(category, lsize, stdin);
    category[strcspn(category, "\n")] = 0;


    printf("Enter the indication of the medicine: ");
    fflush(stdin);
    fgets(indication, lsize, stdin);
    indication[strcspn(indication, "\n")] = 0;


    printf("Enter the company of the medicine: ");
    fflush(stdin);
    fgets(company, lsize, stdin);
    company[strcspn(company, "\n")] = 0;


    printf("Enter the expiry date of the medicine: ");
    fflush(stdin);
    fgets(expiryDate, lsize, stdin);
    expiryDate[strcspn(expiryDate, "\n")] = 0;


    printf("Enter the counter of the medicine: ");
    scanf("%d", &counter);

    root = insertMedicine(root, medicineName, activeIngredient, category, indication, expiryDate, company, counter);
    return root;
}

//TO SEARCH FOR THE MEDICINE WITH THE GIVEN NAME
ptr_medicine searchMedicine(ptr_medicine root, char medicineName[])
{

    if (root == NULL)
    {
        return NULL;
    }
    int compare = strcmp(medicineName, root->medicineName);
    if (compare < 0)
    {
        return searchMedicine(root->left, medicineName);
    }
    else if (compare > 0)
    {
        return searchMedicine(root->right, medicineName);
    }
    else
    {
        return root;
    }

}

//UPDATE THE QUANTITY (COUNTER) OF THE MEDICINE
void updateQuantity( ptr_medicine medicine, int newQuantity)
{

    medicine->counter = newQuantity;

}

//FUNCTION TO ADD THE MEDICINE TO THE LINKED LIST
void addToList(ptr_medicine medicine)
{

    medicine_list_ptr newNode = (medicine_list_ptr)malloc(sizeof(struct medicine_list));
    newNode->medicine = medicine;
    newNode->next = NULL;
    if(head == NULL)
    {
        head = tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }

}

//TO LIST THE MEDICINES IN ALPHABETICAL
void sortedTree(ptr_medicine root)
{

    if (root == NULL) return;
    inOrderTraversal(root);
    medicine_list_ptr curr = head;
    while(curr != NULL)
    {
        printf("Name: %s\n Active Ingredient: %s\n Category: %s\n Indication: %s\n Expiry Date: %s\n Company: %s\n Quantity: %d\n", curr->medicine->medicineName, curr->medicine->activeIngredient, curr->medicine->category, curr->medicine->indication, curr->medicine->expiryDate, curr->medicine->company, curr->medicine->counter);
        curr = curr->next;
    }

}

//TO VISIT ALL THE NODES
void inOrderTraversal(ptr_medicine root){

    if(root != NULL)
    {
        inOrderTraversal(root->left);
        addToList(root);
        inOrderTraversal(root->right);
    }

}

//LIST ALL MEDICINES OF SAME CATEGORY
void listByCategory(ptr_medicine root, char category[])
{
    if (root != NULL)
    {
        listByCategory(root->left, category);
        if (strcmp(root->category, category) == 0)
        {
            displayTree(root);
        }
        listByCategory(root->right, category);
    }
}

//LIST ALL MEDICINES THAT TREAT A SPECIFIC DISEASE
void listByIndication(ptr_medicine temp, char indication[])
{
    if(temp == NULL)
        return;
    char *token= strtok(indication," ");
    while(token != NULL)
    {
        if(strstr(temp->indication, token) != NULL)
        {
            printf("Name: %s\n Active Ingredient: %s\n Category: %s\n Indication: %s\n Expiry Date: %s\n Company: %s\n Quantity: %d\n", temp->medicineName, temp->activeIngredient, temp->category, temp->indication, temp->expiryDate, temp->company, temp->counter);
            break;
        }
        token = strtok(NULL, " ");
    }
    listByIndication(temp->left, indication);
    listByIndication(temp->right, indication);
}

//DELETE MEDICINE FROM THE USER
ptr_medicine deleteMedicine(ptr_medicine root, char medicineName[])
{
    if (root == NULL)
    {
        return root;
    }
    if (strcmp(medicineName, root->medicineName) < 0)
    {
        root->left = deleteMedicine(root->left, medicineName);
    }
    else if (strcmp(medicineName, root->medicineName) > 0)
    {
        root->right = deleteMedicine(root->right, medicineName);
    }
    else
    {
        if (root->left == NULL)
        {
            ptr_medicine temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
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

//FIND THE MIN VALUE OF THE BST
ptr_medicine minValueNode(ptr_medicine root)
{

    ptr_medicine current = root;
    while (current->left != NULL)
    {
        current = current->left;
    }
    return current;

}

//DELETE ALL MEDICINES FROM A SPECIFIC COMPANY
ptr_medicine deleteAsCompany(ptr_medicine root, char company[])
{
    if (root == NULL)
    {
        return root;
        printf("No medicines from the specified company found\n");
    }
    root->left = deleteAsCompany(root->left, company);
    root->right = deleteAsCompany(root->right, company);
    if (strcmp(root->company, company) == 0)
    {
        printf("Deleting medicine %s from company %s\n", root->medicineName, root->company);
        free(root);
        root = NULL;
    }

    return root;
}

//SAVE THE DATA IN NEW FILE
void saveInFile(ptr_medicine root)
{
    FILE* medicinesFile = fopen("Pharmacy.txt", "w");
    if (medicinesFile == NULL)
    {
        printf("Error ! File can not be opened \n");
        return;
    }
    saveInorder(root, medicinesFile);
    fclose(medicinesFile);
    printf("Successfully saved all information to Pharmacy.txt\n");
}

//TO USE IT IN PRINT THE SORTED DATA
void saveInorder(ptr_medicine root, FILE* file)
{
    if (root == NULL)
    {
        return;
    }
    saveInorder(root->left, file);
    fprintf(file, "%s:%s:%s:%s:%s:%d:%s\n", root->medicineName, root->activeIngredient, root->category, root->indication, root->expiryDate, root->counter, root->company);
    saveInorder(root->right, file);
}

//CREATE A NEW HASH TABLE
ptr_hash createHashTable()
{
    ptr_hash ht = (ptr_hash)malloc(sizeof(struct HashTable));
    ht->size = 0;
    for(int i = 0; i < TABLE_SIZE; i++)
    {
        ht->table[i] = NULL;
    }
    return ht;
}

// HASH FUNCTION TO CALCULATE INDEX
int hash(char *medicineName) {
    long hashval = 0;
    for (int i = 0; medicineName[i+2] != '\0'; i++) {
        hashval = medicineName[i] + medicineName[i+1] + medicineName[i+2];
    }
    return hashval % TABLE_SIZE;
}

//ADD A NEW MEDICINE TO THE HASH TABLE
void addMedicineHash(ptr_hash ht, ptr_medicine med)
{
    if (ht->size>=LoadFactor*SIZEHASH){
        ReHash();
    }
    int index = hash(med->medicineName);
    if(ht->table[index] == NULL) {
        ht->table[index] = med;
    } else {
        ptr_medicine cur = ht->table[index];
        while(cur) {
            if(strcmp(med->medicineName, cur->medicineName) < 0) {
                if(cur->left == NULL) {
                    cur->left = med;
                    break;
                } else {
                    cur = cur->left;
                }
            } else {
                if(cur->right == NULL) {
                    cur->right = med;
                    break;
                } else {
                    cur = cur->right;
                }
            }
        }
    }
    ht->size++;
}

// FUNCTION TO READ FROM FILE AND STORE THE DATA IN THE HASH TABLE
ptr_hash readHashFile(ptr_hash hashTable)
{
    const char *filename = "Pharmacy.txt";
    char line[lineSize];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open file: %s", filename);
        return hashTable;
    }

    while (fgets(line, lineSize, fp) != NULL) {
        char *medicineName = strtok(line, ":");
        char *activeIngredient = strtok(NULL, ":");
        char *category = strtok(NULL, ":");
        char *indication = strtok(NULL, ":");
        char *expiryDate = strtok(NULL, ":");
        int counter = atoi(strtok(NULL, ":"));
        char *company = strtok(NULL, ":");

        // Create a new medicine node
        ptr_medicine newMedicine = (ptr_medicine) malloc(sizeof(struct medicine));
        if (newMedicine == NULL) {
            printf("Failed to allocate memory for medicine node");
            return hashTable;
        }
        strncpy(newMedicine->medicineName, medicineName, lsize);
        strncpy(newMedicine->activeIngredient, activeIngredient, lsize);
        strncpy(newMedicine->category, category, lsize);
        strncpy(newMedicine->indication, indication, lsize);
        strncpy(newMedicine->expiryDate, expiryDate, lsize);
        newMedicine->counter = counter;
        newMedicine->left = NULL;
        newMedicine->right = NULL;

        // Hash the medicine name to find the appropriate slot in the hash table
        int hashValue = hash(medicineName);
        ptr_medicine current = hashTable->table[hashValue];

        // If the slot is empty, create a new binary search tree and add the medicine as the root node
        if (current == NULL) {
            hashTable->table[hashValue] = newMedicine;
            hashTable->size++;
        }
            // Otherwise, insert the medicine into the existing binary search tree
        else {
            ptr_medicine parent;
            while (current != NULL) {
                parent = current;
                if (strcmp(medicineName, current->medicineName) < 0) {
                    current = current->left;
                }
                else if (strcmp(medicineName, current->medicineName) > 0) {
                    current = current->right;
                }
                    // If the medicine already exists in the binary search tree, increment the counter
                else {
                    current->counter += counter;
                    free(newMedicine);
                    break;
                }
            }
            if (current == NULL) {
                if (strcmp(medicineName, parent->medicineName) < 0) {
                    parent->left = newMedicine;
                }
                else {
                    parent->right = newMedicine;
                }
            }
        }
    }

    fclose(fp);
    return hashTable;
}


//PRINT HASHED TABLE
void printHashTable(ptr_hash ht)
{
    for(int i = 0; i < TABLE_SIZE; i++) {
        ptr_medicine cur = ht->table[i];
        printf("Index %d:\n", i);
        while(cur) {
            printf("Medicine Name: %s\n", cur->medicineName);
            printf("Active Ingredient: %s\n", cur->activeIngredient);
            printf("Category: %s\n", cur->category);
            printf("Indication: %s\n", cur->indication);
            printf("Expiry Date: %s\n", cur->expiryDate);
            printf("Counter: %d\n", cur->counter);
            printf("Company: %s\n", cur->company);
            cur = cur->left;
    }
    }
}

//SEARCH ABOUT MEDICINE IN THE HASH TABLE
ptr_medicine searchMedicineInHashTable(ptr_hash ht, char* medicineName)
{
    unsigned int index = hash(medicineName);
    ptr_medicine cur = ht->table[index];
    while(cur) {
        if(strcmp(medicineName, cur->medicineName) == 0) {
            return cur;
        }
        if(strcmp(medicineName, cur->medicineName) < 0) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return NULL;
}

//PRINT THE USED HASH FUNCTION
void printHashFunction(ptr_hash ht) {
    printf("int hash(char *medicineName) {\n"
           "    unsigned long hashval = 0;\n"
           "    for (int i = 0; medicineName[i+2] != '\\0'; i++) {\n"
           "        hashval = medicineName[i] + medicineName[i+1] + medicineName[i+2];\n"
           "    }\n"
           "    return hashval % TABLE_SIZE;\n"
           "}");
}

//ADD RECORD (MEDICINE) IN THE HASH TABLE FROM THE USER
void addRecord(ptr_hash ht) {
    ptr_medicine med = (ptr_medicine)malloc(sizeof(struct medicine));
    printf("Enter the medicine name: ");
    scanf("%s", med->medicineName);
    printf("Enter the active ingredient: ");
    scanf("%s", med->activeIngredient);
    printf("Enter the category: ");
    scanf("%s", med->category);
    printf("Enter the indication: ");
    scanf("%s", med->indication);
    printf("Enter the expiry date: ");
    scanf("%s", med->expiryDate);
    printf("Enter the counter: ");
    scanf("%d", &med->counter);
    printf("Enter the company: ");
    scanf("%s", med->company);
    addMedicineHash(ht, med);
}

//DELETE A SPECIFIC RECORD
ptr_hash deleteMedicineHash(ptr_hash ht, char* medicineName) {

    unsigned int index = hash(medicineName);
    ptr_medicine cur = ht->table[index];
    ptr_medicine prev = NULL;
    while(cur) {
        if(strcmp(medicineName, cur->medicineName) == 0) {
            if(prev == NULL) {
                ht->table[index] = cur->right;
            } else {
                prev->right = cur->right;
            }
            free(cur);
            ht->size--;
            return ht;
        }
        prev = cur;
        cur = cur->right;
    }
    printf("Medicine not found in the Hash Table\n");
    return ht;
}

void saveHashToFile(ptr_hash ht) {
    FILE* fp = fopen("Pharmacy.txt", "w");
    if (!fp) {
        printf("Error opening file for writing\n");
        return;
    }
    for (int i = 0; i < ht->size; i++) {
        ptr_medicine cur = ht->table[i];
        while (cur) {
            fprintf(fp, "%s:%s:%s:%s:%s:%d:%s\n", cur->medicineName, cur->activeIngredient,cur->category,cur->indication,cur->expiryDate, cur->counter,cur->company);
            cur = cur->right;
        }
    }
    fclose(fp);
}


int main()
{

    ptr_medicine root = (ptr_medicine)malloc(sizeof(struct medicine));
    root = NULL ;
    ht = createHashTable();
    display_menu(root,ht); // DISPLAY THE OPTIONS TO THE USER
    return 0;

}
void ReHash(){
    int new_size = ht->size * 2;
    ptr_hash new_table = (ptr_hash) calloc(new_size, sizeof(struct HashTable));
    for (int i = 0; i < ht->size; i++) {
        if (strcmp(ht->table[i]->medicineName,"")!=0) {

            int index = hash(ht->table[i]->medicineName);
            while (strcmp(new_table->table[i]->medicineName,"")!=0) {

                index = (index + 1) % new_size;
            }
            new_table->table[i]= ht->table[i];
        }
    }
    free(ht);
    ht = new_table;
    ht->size = new_size;
}
