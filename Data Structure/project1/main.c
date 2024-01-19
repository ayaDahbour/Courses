#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define READ_MAX 50
#define Name_Max 20
#define Code_Max 3

struct Players {  // players linked list
    char Name [Name_Max];
    struct Players *Next;
};
struct Tnode { // team linkedlist
    char Name [Name_Max];
    char Code [Code_Max];
    int Wins;
    int Draws;
    int Loses;
    int Goal_Difference;
    int NumberOfPoints;
    struct Tnode *Next;
    struct Players *Elementary_Players;
    struct QueuePlayers *Spare;
};
struct QueuePlayers { // queue of spare players
    char Name[Name_Max];
    struct QueuePlayers *Next;
    struct QueuePlayers *Last;
};
void DeleteTeamList(struct Tnode *Deleted_List);// delete team list
void RedCard(struct Tnode *Team,char TeamCode[Code_Max]);
int PointsCalculator(int Wins, int Draws); // calculate points
void DeleteTeam(char code[3], struct Tnode* Teams);
int IsLastTeam (struct Tnode* Teams);// is last?
void PrintTeamsList(struct Tnode* Teams);// print all list
void InsertTeam(char Read[READ_MAX], struct Tnode *Team);// insert teams information
void menu(struct Tnode *Teams);
void Read(FILE *ptr, char Code, struct Tnode *T); // read file
void Radix (struct Tnode *Team); // sort by radixSort
void Change(struct Tnode *A,struct Tnode *Team);// change element to the linked list
void InsertNewTeam(struct Tnode *Team); //insert new Team
void ModifyTeam (struct Tnode *Team);
void PrintTeams2File(struct Tnode *Team);//save Teams information into new file
void ReadPlayersFromFile(struct Tnode *Team);// read players information from file
void InsertPlayer(struct Tnode *Team,char Name [Name_Max]);//add new player to the list
void PrintAllPlayers(struct Tnode *Team);//print all players
void MakeAMatch(struct Tnode *Team);// make a match
void InsertPlayerS(struct Tnode *Team,char Name [Name_Max]);//insert spare player
void SavePlayersIntoFile(struct Tnode *Team);//save players data into file
void DeletePlayer(struct Players *Deleted, struct Players *Players);//delete player node
void switchPlayers(struct Tnode *Team,char TeamCode[Code_Max]);//switch players between spare and elementary list
void PrintPlayers(struct Tnode *Team);// print all players in the team
char Read_line[READ_MAX];
char *token;

int main() {
    struct Tnode *TeamHeader = (struct Tnode*)(malloc(sizeof (struct Tnode)));
    TeamHeader->Next=NULL;
    menu(TeamHeader); //call the menu first
    return 0;
}
void DeleteTeamList(struct Tnode *Deleted_List) {
    struct Tnode *node, *temp;
    node = Deleted_List-> Next;
    Deleted_List-> Next = NULL;
    while (node != NULL) {
        temp = node-> Next;
        free(node);
        node = temp;
    }
}
void InsertTeam(char Read[READ_MAX], struct Tnode *Team){
    struct Tnode* temp=Team;
    struct Tnode *newNode;
    newNode= malloc(sizeof (struct Tnode));
    int counting=0;
    token=strtok(Read, ",");
    while (token!=NULL) {
        if (counting == 0) {
            strcpy(newNode->Name, token);
        } else if (counting ==1) {
            strcpy(newNode->Code, token);
        } else if (counting ==2) {
            newNode->Wins = atoi(token);
        } else if (counting ==3) {
            newNode->Draws = atoi(token);
        } else if (counting ==4) {
            newNode->Loses = atoi(token);
        } else if (counting ==5) {
            newNode->Goal_Difference = atoi(token);
        }
        token=strtok(NULL, ",");
        counting++;
    }
    newNode->NumberOfPoints = PointsCalculator(newNode->Wins, newNode->Draws);
    while (temp->Next!=NULL){
        temp=temp->Next;
    }
    temp->Next=newNode;
}
int PointsCalculator(int Wins, int Draws){
    return ( ( 3 * Wins ) + ( Draws ) );
}
void PrintTeamsList(struct Tnode* Teams){
    char space=' ';
    struct Tnode* Position = Teams;
    if(IsLastTeam(Teams)) {
        printf("Error ! Empty list\n");
    }
    else {
        do {
            Position = Position-> Next;
            printf("%-10s %-10s %-10d %-11d %-11d %-21d %d\n", Position-> Name, Position->Code, Position->Wins,
                   Position->Draws, Position->Loses, Position->Goal_Difference, Position->NumberOfPoints);

        }
        while (!IsLastTeam(Position));
    }
    printf("\n");
}
int IsLastTeam (struct Tnode *Teams){
    return Teams->Next==NULL;
}
void menu (struct Tnode *Teams){
    int option=0;
    char Code[Code_Max];
    char teamCode[3];
    FILE *TeamFile=fopen("teams.txt","r");
    while (1){
        printf("************ you can not choose 3-8 before choose 1&2**********************\n");
        printf("1. Load the team information file\n"); // load and print teams *******done
        printf("2. Load the players information file\n"); // load and print players //done
        printf("3. Printing all teams with their relevant information\n"); //***done
        printf("4. Sort teams information \n"); //radix sort
        printf("5. Save in teams in File\n");
        printf("6. Edit team information\n");//***done
        printf("7. Add new team \n");//****done
        printf("8. Delete team \n");//done
        printf("9. Create a match \n");//***done
        printf("10. Switch between elementary player and spare player\n");//done
        printf("11. Red cart !! \n"); // delete form elementary//done
        printf("12. Print  all elementary players for a specific team\n");//done
        printf("13. Save players information in new file  \n"); //"playersInfo.txt"//done
        printf("14. Exit \n");
        printf("Your choice : ");
        printf("\n");
        scanf("%d",&option);
        switch (option) {
            case 1:
                Read(TeamFile,'T',Teams);
                break;
            case 2:
                ReadPlayersFromFile(Teams);
                break;
            case 3:
                PrintTeamsList(Teams);
                break;
            case 4:
                Radix(Teams);
                break;
            case 5:
                PrintTeams2File(Teams);
                break;
            case 6:
                ModifyTeam (Teams);
                break;
            case 7:
                InsertNewTeam(Teams);
                break;
            case 8:
                printf("Please enter the code for the team you want to delete\n");
                scanf("%s",teamCode);
                DeleteTeam(teamCode,Teams);
                break;
            case 9:
                MakeAMatch(Teams);
                break;
            case 10:
                printf("Please enter Team code:");
                scanf("%s",Code);
                switchPlayers(Teams,Code);
                break;
            case 11:
                printf("Please enter Team code:");
                scanf("%s",Code);
                RedCard(Teams,Code);
                break;
            case 12:
                PrintPlayers(Teams);
                break;
            case 13:
                SavePlayersIntoFile(Teams);
                break;
            case 14:
                DeleteTeamList(Teams);
                fclose(TeamFile);
                exit(1);
            default:
                break;

        }


    }
}
void Read(FILE *ptr, char Code, struct Tnode *T) {
    if (ptr == NULL) {
        printf("There is no file :)\n");
    } else {
       // read all lines from the file and save it in linkedlist
        while (fgets(Read_line, READ_MAX, ptr) != NULL) {
            if (Code=='T'){
                InsertTeam(Read_line,T);
            }
            token=NULL;
        }

    }
}
int getMaxPoints(struct Tnode *Team){
    struct Tnode *temp=Team->Next;
    int max=temp->NumberOfPoints;
    while (temp->Next!=NULL){
        temp = temp->Next;
        if (max < temp->NumberOfPoints){
            max = temp->NumberOfPoints;
        }
    }
    return max;
}
void Radix (struct Tnode *Team){
    struct Tnode *A[10],*temp,*t;
    int count=0,max= getMaxPoints(Team);
    int Points;
    int division=1;
    while (max!=0){ // max number of points
        max=max/10;
        count++;
    }
    for (int i = 0; i < 10; i++) {
        A[i] = (struct Tnode *) malloc(sizeof(struct Tnode));
        A[i]->Next = NULL;
    }
    while(count!=0) {   // repeat the process n times
        temp=Team;
        count--;
        while (temp->Next != NULL) {
            temp = temp->Next;
            Points = (temp->NumberOfPoints)/division;
            Change(A[Points % 10], temp);
        }
        temp = Team;
        for (int i = 0; i < 10; i++) { // every index
            t = A[i]->Next;
            A[i]->Next = NULL;
            if (t == NULL) { // if the node is empty
                continue;
            } else {
                temp->Next = t;
                while (temp->Next != NULL) {
                    temp = temp->Next;
                }
            }
        }
        division=division*10;
    }
}
void Change(struct Tnode *A,struct Tnode *Team){
    struct Tnode *newNode= malloc(sizeof( struct Tnode));
    struct Tnode *temp=A;
    struct Tnode *temp2;
    newNode->Next=NULL;
    strcpy(newNode->Name,Team->Name);
    strcpy(newNode->Code,Team->Code);
    newNode->NumberOfPoints=Team->NumberOfPoints;
    newNode->Draws=Team->Draws;
    newNode->Wins=Team->Wins;
    newNode->Loses=Team->Loses;
    newNode->Goal_Difference=Team->Goal_Difference;
    while (temp->Next != NULL) {
        temp2=temp;
        temp = temp->Next;
        if (temp->Goal_Difference > newNode->Goal_Difference && temp->NumberOfPoints == newNode->NumberOfPoints){ // chick the goal diff
            temp=temp2;
            break;
        }
    }
    temp2=temp->Next;
    temp->Next = newNode;
    temp=temp->Next;
    temp->Next=temp2;
}
void DeleteTeam(char code[3], struct Tnode* Teams){
    struct Tnode*temp=Teams;
    struct Tnode *temp1,*temp2;
    int check=0;
    while (temp->Next!=NULL){ //throw every node
        temp2=temp;
        temp=temp->Next;
        if (strcmp(temp->Code,code)==0){
            temp1=temp->Next;
            free(temp);
            temp2->Next=temp1;
            check=1;
            break;
        }
    }
    if(check==0){ // search if that exist node with this code
        printf("You have entered invalid code!\n");
    }
}
void InsertNewTeam(struct Tnode *Team){
    struct Tnode *newNode=(struct Tnode*)malloc(sizeof( struct Tnode));
    newNode->Next=NULL;
    struct Tnode *temp=Team;
    printf("Please enter new team name:");
    scanf("%s",newNode->Name);
    printf("Please enter new team code:");
    scanf("%s",newNode->Code);
    printf("Please enter new team Wins:");
    scanf("%d",&newNode->Wins);
    printf("Please enter new team Draws:");
    scanf("%d",&newNode->Draws);
    printf("Please enter new team loses:");
    scanf("%d",&newNode->Loses);
    printf("Please enter new team Goal Difference:");
    scanf("%d",&newNode->Goal_Difference);
    newNode->NumberOfPoints= PointsCalculator(newNode->Wins,newNode->Draws);
    while (temp->Next != NULL) {
        temp = temp->Next;
    }
    temp->Next = newNode;
}
void ModifyTeam (struct Tnode *Team){
    struct Tnode *temp=Team;
    char code [3],Name[Name_Max];
    int found=0,choice,update;
    printf("\nPlease enter the code for the team you want to edit: ");
    scanf("%s",code);
    while(temp->Next!=NULL){
        temp = temp->Next;
        if (strcmp(temp->Code,code)==0){
            found=1;
            break;
        }
    }
    if(found!=1){   // check if the team is found or no
        printf("Sorry the team does not found!\n");
    }
    else{
        printf("1.Name\n");
        printf("2.Code\n");
        printf("3.Wins\n");
        printf("4.Draws\n");
        printf("5.Loses\n");
        printf("6.Goal difference\n");
        printf("Please enter what do you want to change in numbers:\n");
        scanf("%d",&choice);
        switch (choice) { //To change the data depending on the choice
            case 1:
                printf("Please enter the new name");
                scanf("%s",Name);
                strcpy(temp->Name,Name);
                break;
            case 2:
                printf("Please enter the new code");
                scanf("%s",code);
                strcpy(temp->Code,code);
                break;
            case 3:
                printf("Please enter the new number of wins");
                scanf("%d",&update);
                temp->Wins=update;
                temp->NumberOfPoints=PointsCalculator(temp->Wins,temp->Draws);
                break;
            case 4:
                printf("Please enter the new number of draws");
                scanf("%d",&update);
                temp->Draws=update;
                temp->NumberOfPoints=PointsCalculator(temp->Wins,temp->Draws);

                break;
            case 5:
                printf("Please enter the new number of loses");
                scanf("%d",&update);
                temp->Loses=update;
                break;
            case 6:
                printf("Please enter the new goal difference");
                scanf("%d",&update);
                temp->Goal_Difference=update;
                break;
            default:
                break;
        }
    }
}
void PrintTeams2File(struct Tnode *Team){
    FILE *ptr= fopen("teamsInfo.txt","w");
    struct Tnode *temp=Team;
    while (temp->Next!=NULL){
        temp=temp->Next;
        fprintf(ptr,"%s,%s,%d,%d,%d,%d\n",temp->Name,temp->Code,temp->Wins,temp->Draws,temp->Loses,temp->Goal_Difference);
    }
    fclose(ptr);
}
void ReadPlayersFromFile(struct Tnode *Team){
    FILE *ptr= fopen("players.txt","r");
    char Line [READ_MAX],code[4],Name[Name_Max];
    int found=0;
    struct Tnode *temp;
    while(fgets(Line,READ_MAX,ptr)){ // read every line
        if (Line[0]=='*'){
            found=0;
            token= strtok(Line,"*");
            temp=Team;
            strcpy(code,token--);
            code[3]='\0';
            while(temp->Next!=NULL){
                temp=temp->Next;
                if(strcmp(temp->Code,code)==0){
                    found=1;
                    temp->Elementary_Players=(struct Players*)(malloc(sizeof (struct Players)));
                    temp->Spare =(struct QueuePlayers*)(malloc(sizeof (struct QueuePlayers)));
                    break;
                }
            }
        }
        else if (found){//to check if the team is found or not
            token= strtok(Line,"-");
            if (atoi(token)<=11){
                token= strtok(NULL,"-");
                strcpy(Name,token);
                Name[strlen(Name)-1]='\0';
                InsertPlayer(temp,Name);
            }
            else{
                token= strtok(NULL,"-");
                strcpy(Name,token);
                Name[strlen(Name)-1]='\0';
                InsertPlayerS(temp,Name);

            }
        }
    }
}
void InsertPlayerS(struct Tnode *Team,char Name [Name_Max]){
    struct QueuePlayers *temp,*newNode=(struct QueuePlayers*)(malloc(sizeof (struct QueuePlayers)));
    newNode->Next=NULL;
    newNode->Last=NULL;
    temp=Team->Spare;
    while(temp->Next!=NULL){ // in every node
        temp=temp->Next;
    }
    strcpy(newNode->Name,Name);
    temp->Next=newNode;
    temp=Team->Spare;
    while(temp->Next!=NULL){
        temp->Last=newNode;
        temp=temp->Next;
    }
}
void InsertPlayer(struct Tnode *Team,char Name [Name_Max]){
    struct Players *temp,*newNode=(struct Players*)(malloc(sizeof (struct Players)));
    newNode->Next=NULL;
    temp=Team->Elementary_Players;
    while(temp->Next!=NULL){
        temp=temp->Next;
    }
    strcpy(newNode->Name,Name);
    temp->Next=newNode;
}
void PrintAllPlayers(struct Tnode *Team){
    struct Tnode *temp=Team;
    struct Players *temp1;
    struct QueuePlayers *temp2;
    while (temp->Next!=NULL){
        temp=temp->Next;
        temp1=temp->Elementary_Players;
        temp2=temp->Spare;
        if(temp1==NULL){
            continue;
        }
        else {
            if (temp->Elementary_Players->Next!=NULL) { // check if there is elementary players in this team
                int number=1;
                printf("Elementary Players for %s:\n",temp->Name);
                while (temp1->Next != NULL) {
                    temp1 = temp1->Next;
                    printf("%d-%s\n",number, temp1->Name);
                    number++;
                }
            }
        }
    }
}
void MakeAMatch(struct Tnode *Team){
    char code1[3], code2[3];
    int score1,score2,found1=0,found2=0;
    struct Tnode *temp1=Team,*temp2=Team;
    printf("Please enter teams codes for the match:\n");
    printf("Please enter team 1 code:\n");
    scanf("%s",code1);
    while (temp1->Next!=NULL){
        temp1=temp1->Next;
        if (strcmp(temp1->Code,code1)==0){
            found1=1;
            break;
        }
    }
    if (!found1){ // check if the first team is found or not
        printf("There is no team with that code\n");
    }
    else {
        printf("Please enter team 2 code:\n");
        scanf("%s", code2);
        while (temp2->Next!=NULL){
            temp2=temp2->Next;
            if (strcmp(temp2->Code,code2)==0){
                found2=1;
                break;
            }
        }
        if (!found2){// check if the second team is found or not
            printf("There is no team with that code\n");
        }else{
            printf("Please enter team %s scores:\n",code1);
            scanf("%d",&score1);
            printf("Please enter team %s scores:\n",code2);
            scanf("%d",&score2);
            temp1->Goal_Difference+=score1-score2;
            temp2->Goal_Difference+=score2-score1;
            if (score1-score2>0){
                temp2->Loses++;
                temp1->Wins++;
            }
            else if(score1-score2<0){
                temp1->Loses++;
                temp2->Wins++;
            }
            else{
                temp2->Draws++;
                temp1->Draws++;
            }
            // check who is the winner of the match or if there is a draw
            temp1->NumberOfPoints= PointsCalculator(temp1->Wins,temp1->Draws);
            temp2->NumberOfPoints= PointsCalculator(temp2->Wins,temp2->Draws);
        }
    }
}
void SavePlayersIntoFile(struct Tnode *Team){
    FILE *ptr = fopen("playersInfo.txt","w");
    struct Tnode *temp=Team;
    struct Players *temp1;
    struct QueuePlayers *temp2;
    int number;
    while (temp->Next!=NULL){
        number=1;
        temp=temp->Next;
        temp1=temp->Elementary_Players;
        temp2=temp->Spare;
        if(temp1==NULL){ //to check if there is players list or not
            continue;
        }
        else {
            fprintf(ptr,"*%s\n",temp->Code);
            if (temp->Elementary_Players->Next!=NULL) {//check if there is Elementary players
                for (;temp1->Next!=NULL;number++){// every node players elements
                    temp1=temp1->Next;
                    fprintf(ptr,"%d-%s",number,temp1->Name);
                }
            }
            if (temp->Spare->Next!=NULL){ // check if there is spare players
                for (;temp2->Next!=NULL;number++){ // every queue players elements
                    temp2=temp2->Next;
                    fprintf(ptr,"%d-%s",number,temp2->Name);
                }
            }
        }
    }
    fclose(ptr);
}
void RedCard(struct Tnode *Team,char TeamCode[Code_Max]){
    struct Tnode *temp=Team;
    struct Players *temp1,*tmp;
    struct QueuePlayers *temp2,*new=(struct QueuePlayers*) malloc(sizeof(struct QueuePlayers));
    while (temp->Next!=NULL){
        temp=temp->Next;
        if(strcmp(TeamCode,temp->Code)==0) {
            break;
        }
    }
    temp1=temp->Elementary_Players;
    tmp=temp->Elementary_Players;
    PrintPlayers(temp);
    int count=1,number;
    printf("Please enter the number of the player:");
    scanf("%d",&number);
    while(temp1->Next!=NULL){
        temp1=temp1->Next;
        if(count==number){
            break;
        }
        count++;
    }
    temp2=temp->Spare;
    strcpy(new->Name,temp1->Name);
    while(temp2->Next!=NULL){
        temp2=temp2->Next;
        temp2->Last=new;
    }
    temp2->Next=new;
    DeletePlayer(temp1,tmp);
}
void DeletePlayer(struct Players *Deleted, struct Players *Players){
    while (Players->Next!=Deleted){
        Players=Players->Next;
    }
    Players->Next=Deleted->Next;
    free(Deleted);
}
void switchPlayers(struct Tnode *Team,char TeamCode[Code_Max]){
    struct Tnode *temp=Team;
    char Name[Name_Max];
    struct Players *temp1;
    struct QueuePlayers *temp2,*tmp=(struct QueuePlayers*) malloc(sizeof(struct QueuePlayers));
    tmp->Next=NULL;
    while (temp->Next!=NULL){
        temp=temp->Next;
        if(strcmp(TeamCode,temp->Code)==0) {
            break;
        }
    }
    temp1=temp->Elementary_Players;
    temp2=temp->Spare;
    PrintPlayers(temp);
    int count=1,number;
    printf("Please enter the number of the player:");
    scanf("%d",&number);
    while(temp1->Next!=NULL){ // in every node
        temp1=temp1->Next;
        if(count==number){
            break;
        }
        count++;
    }
    temp2=temp2->Next;
    strcpy(Name,temp2->Name);
    temp2=temp->Spare;
    temp->Spare=temp->Spare->Next;
    free(temp2);
    temp2=temp->Spare->Last;
    strcpy(tmp->Name,temp1->Name);
    temp2->Next=tmp;
    temp2=temp->Spare;
    while(temp2->Next!=NULL){ // in every node
        temp2->Last=tmp;
        temp2=temp2->Next;
    }
    strcpy(temp1->Name,Name);
}
void PrintPlayers(struct Tnode *Team){
    struct Players *temp1;
    struct Tnode *temp=Team;
    char code[3];
    int number=0;
    printf("enter team code:");
        scanf("%s",code);
    while (temp->Next!=NULL){
    temp=temp->Next;
    temp1=temp->Elementary_Players;
    if (strcmp(temp->Code,code)==0) { // check if the team has players or not
        number=1;
        printf("Elementary Players for %s:\n",Team->Name);
        while (temp1->Next != NULL) { //  every player node
            temp1 = temp1->Next;
            printf("%d-%s\n",number, temp1->Name);
            number++;
        }
    }
    }
    if (number==0){
        printf("There is no team players");
    }
}
