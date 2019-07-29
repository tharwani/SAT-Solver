#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
// this function converts the parameters into literals where i+1 is the entry in the sudoku,
// j+1 is the row no and k+1 is the column no. where 0<=i,j,k<9
int s_no(int i,int j,int k){
    return (i*81)+(j*9)+(k*1)+1;
}
// to reverse a number
int rev(int a){
    int y = 0;
    int i = 1;
    while(a/(int)pow(10,i)!=0) i++;
    i = i-1;
    for(int j=i;j>=0;j--){
        y += (a/(int)pow(10,j))*(int)pow(10,i-j);
        a = a%(int)pow(10,j);
    }
    return y;
}
int main(int argc, char *argv[]){
    FILE* fptr = fopen("input.txt","w");  // opening the input file where input to the minisat is to be written
    char c[20]; // to scan input from the given input file
    int sud[81]; // filled entries from the given input file will be stored in this using literal no
                 // which will preserve the value and position of the sudoku entry 
    int s = 0;   // keeps track of length of sud
    int i = 0;
    FILE* fptr2 = fopen("p1_input.txt","r"); // open the provided input to file to read
    while(i<9){
        fgets(c,20,fptr2);
        // extracting literals out of the c
        int j = 0;
        while(c[j]!='\0'){
            if(c[j]>='1' && c[j]<='9'){
                sud[s] = s_no(c[j]-'1',i,j);
                ++s;
            }
            ++j;
        }
        i++;
    }
    fclose(fptr2);

    // All possible clauses for a sudoku encoding will be stored in buffer(string) which will be pused into input.txt in the end
     
    int j,k;
    int cnt = 0; // keeps track no of clauses of sudoku encoding
    char buffer[1000000]; // stores the clauses
    // all clauses for the condition where each entry has at least 1 number in it from [1,9]
    for(j=0;j<9;j++){
        for(k=0;k<9;k++){
            for(i=0;i<9;i++){
                char temp[10];
                sprintf(temp,"%d ",s_no(i,j,k));
                strcat(buffer,temp);
            }
            char temp[10];
            sprintf(temp,"0\n");
            strcat(buffer,temp);
            cnt++;
        }
    }
    // all clauses for the condition where each entry has at most 1 number in it from [1,9]
    for(j=0;j<9;j++){
        for(k=0;k<9;k++){
            for(i=0;i<9;i++){
                for(int l=0;l<i;l++){
                    char temp[30];
                    sprintf(temp,"%d %d %d\n",-(s_no(l,j,k)),-(s_no(i,j,k)),0);
                    strcat(buffer,temp);
                    cnt++;
                }
            }
        }
    }
    // all clauses for no two entries in a row have same value
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            for(k=0;k<9;k++){
                for(int l=0;l<k;l++){
                    char temp[30];
                    sprintf(temp,"%d %d %d\n",-(s_no(i,j,l)),-(s_no(i,j,k)),0);
                    strcat(buffer,temp);
                    cnt++;
                }
            }
        }
    }
    // all clauses for no two entries in a column have same value
    for(i=0;i<9;i++){
        for(k=0;k<9;k++){
            for(j=0;j<9;j++){
                for(int l=0;l<j;l++){
                    char temp[30];
                    sprintf(temp,"%d %d %d\n",-(s_no(i,l,k)),-(s_no(i,j,k)),0);
                    strcat(buffer,temp);
                    cnt++;
                }
            }
        }
    }
    // all clauses for no two entries in the main left diagonal have same value
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            for(int l=0;l<j;l++){
                char temp[30];
                sprintf(temp,"%d %d %d\n",-(s_no(i,l,l)),-(s_no(i,j,j)),0);
                strcat(buffer,temp);
                cnt++;
            }
        }
    }
    // all clauses for no two entries in the main right diagonal have same value
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            for(int l=0;l<j;l++){
                char temp[30];
                sprintf(temp,"%d %d %d\n",-(s_no(i,l,8-l)),-(s_no(i,j,8-j)),0);
                strcat(buffer,temp);
                cnt++;
            }
        }
    }
    // all clauses for no two entries inside each box have same value
    for(i=0;i<9;i++){
        for(int m=0;m<3;m++){
            for(int n=0;n<3;n++){
                for(j=3*m;j<3*(m+1);j++){
                    for(k=3*n;k<3*(n+1);k++){
                        for(int p=3*m;p<j;p++){
                            for(int q=3*n;((q<k)||(p<j))&&(q<3*(n+1));q++){
                                char temp[30];
                                sprintf(temp,"%d %d %d\n",-(s_no(i,p,q)),-(s_no(i,j,k)),0);
                                strcat(buffer,temp);
                                cnt++;
                            }
                        }
                    }
                }
            }
        }
    }
    // pushing clauses that can be formed by sud in buffer
    for(i=0;i<s;i++){
        char temp[30];
        sprintf(temp,"%d 0\n",sud[i]);
        strcat(buffer,temp);
    }
    // writing input in input.txt
    fprintf(fptr,"p cnf 729 %d\n",s+cnt);
    fprintf(fptr,"%s",buffer);
    fclose(fptr);

    // invoking minisat and taking output in output.txt
    char* cmd = "minisat input.txt output.txt";
    system(cmd);

    // decode the output and print the sudoku 
    char c1[10];
    char c2[10000]; // stores the output
    fptr = fopen("output.txt","r"); // open the oputput in read mode
    if(fptr==NULL){
        printf("Error in opening file");
        exit(1);
    }
    fgets(c1,10000,fptr);
    fgets(c2,100000,fptr);
    if(strcmp(c1,"SAT\n")!=0){
        printf("The given encoding is unsatisfiable.\n");
        exit(1);
    }
    // solution will store the sudoku in linear and encoded form
    int solution[81];
    // To extract out useful encoding literals from c2 in solution
    s = 0;
    i=0;
    if(c2[i]=='1'){
        solution[s] = 1;
        s++;
    }
    while(c2[i]!='\n'){
        if((c2[i]==' ')&&(c2[i+1]!='-')&&(c2[i+1]!='0')){
            int j;
            for(j=1;c2[i+j]!=' ';j++){
                solution[s] += (c2[i+j]-'0')*(int)pow(10,j-1);
            }
            solution[s] = rev(solution[s]);
            if(c2[i+j-1]=='0'&&c2[i+j-2]=='0') solution[s] = solution[s]*100;
            else if(c2[i+j-1]=='0') solution[s]=solution[s]*10;
            s++;
            i = i+j-1;
        }
        i++;
    }
    for(i=0;i<81;i++){
        solution[i] -= 1;
    }
    // sudoku will store the decoded form of solution
    int sudoku [9][9];
    // decoding solution in sudoku
    for(i=0;i<81;i++){
        int m = solution[i]/81;
        solution[i] = solution[i]%81;
        int n = solution[i]/9;
        solution[i] = solution[i]%9;
        int o = solution[i];
        sudoku[n][o] = m+1;
    }
    // print the ouptut sudoku
    for(i=0;i<9;i++){
        for(int j=0;j<9;j++){
            printf("%d ",sudoku[i][j]);
        }
        printf("\n");
    }
    return 0;
}