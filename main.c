#include <stdlib.h>
#include <stdio.h>
#include "main.h"


typedef unsigned char byte;
typedef int error_code;

#define ERROR (-1)
#define HAS_ERROR(code) ((code) < 0)
#define HAS_NO_ERROR(code) ((code) >= 0)

/**
 * Cette fonction compare deux chaînes de caractères.       
 * @param p1 la première chaîne
 * @param p2 la deuxième chaîne
 * @return le résultat de la comparaison entre p1 et p2. Un nombre plus petit que
 * 0 dénote que la première chaîne est lexicographiquement inférieure à la deuxième.
 * Une valeur nulle indique que les deux chaînes sont égales tandis qu'une valeur positive
 * indique que la première chaîne est lexicographiquement supérieure à la deuxième.
 */
int strcmp(char *p1, char *p2) {
    char *s1 = (char *) p1;
    char *s2 = (char *) p2;
    char c1, c2;
    do {
        c1 = (char) *s1++;
        c2 = (char) *s2++;
        if (c1 == '\0')
            return c1 - c2;
    } while (c1 == c2);
    return c1 - c2;
}

/**
 * Ex. 1: Calcul la longueur de la chaîne passée en paramètre selon
 * la spécification de la fonction strlen standard
 * @param s un pointeur vers le premier caractère de la chaîne
 * @return le nombre de caractères dans le code d'erreur, ou une erreur
 * si l'entrée est incorrecte
 */
error_code strlen2(char *s) {
    if (s) {
        int lenStr = 0;
        //not to count null
        while ((*s) && (*s != EOF) && (*s !='\r') && (*s !='\n') && (*s != '\0')){
            lenStr += 1;
            s++;
        }
        if (lenStr >= 0) {
            return lenStr;
        }
    }
    return ERROR;
}

/**
 * Ex.2 :Retourne le nombre de lignes d'un fichier sans changer la position
 * courante dans le fichier.
 * @param fp un pointeur vers le descripteur de fichier
 * @return le nombre de lignes, ou -1 si une erreur s'est produite
 */
error_code no_of_lines(FILE *fp) {
    int ch;
    int lines = 0;
    int pt = ftell(fp);

    if(fp == NULL){
        return ERROR;
    }

    for (ch = getc(fp); ch != EOF; ch = getc(fp)){
        if((ch != '\n') && (ch = getc(fp)) == EOF) {
            lines++;
        }
        if (ch == '\n'){
            lines++;
        }
    }

    fseek(fp, 0, SEEK_SET);
    fseek(fp,pt,SEEK_CUR);

    return lines;
}

/**
 * Ex.3: Lit une ligne au complet d'un fichier
 * @param fp le pointeur vers la ligne de fichier
 * @param out le pointeur vers la sortie
 * @param max_len la longueur maximale de la ligne à lire
 * @return le nombre de caractère ou ERROR si une erreur est survenue
 */
error_code readline(FILE *fp, char **out, size_t max_len) {
    //check if null pointers
    if ((!fp) || (!out) || (!max_len)){
        return ERROR;
    }

    //alloue la memoire juste si le pointeur est null pour eviter de dereferencier un pointeur deja actif
    if (!*out){
        //+1 pour ajouter Null a la fin de la ligne (Null terminated Sring)
        *out = malloc((max_len + 1));
    }

    //si malloc retourne Null, donc pas de memoire allouee
    if (!*out){
        return ERROR;
    }

    size_t nbCarNoNulNoNewline=0;
    int charToCompar;
    char charGetc;

    for (size_t compte = 0; (compte <= max_len) ; compte++){
        charToCompar = getc(fp);
        charGetc = (char)charToCompar;

        if ((charGetc != '\n') && (charGetc != '\0') && (charGetc != '\r') && (charToCompar != -1) && (charGetc != EOF)){
            (*out)[nbCarNoNulNoNewline] = charGetc;
            nbCarNoNulNoNewline ++;
        }

        // A la fin de la ligne (ou du fichier) on quitte la loop
        if ((charGetc == '\n') ||(charGetc == '\0')||(charGetc == '\r')|| (feof(fp) )){
            break;
        }
    }

    for(int nbLetNul = nbCarNoNulNoNewline; nbLetNul<=(max_len-nbCarNoNulNoNewline); nbLetNul++){
        (*out)[nbLetNul] = '\0';
    }//(*out)[max_len+1] = '\0';

    return nbCarNoNulNoNewline;
}

/**
 * Ex.4: Copie un bloc mémoire vers un autre
 * @param dest la destination de la copie
 * @param src  la source de la copie
 * @param len la longueur (en byte) de la source
 * @return nombre de bytes copiés ou une erreur s'il y a lieu
 */
error_code memcpy2(void *dest, void *src, size_t len) {
       //check if null pointers
        if ((!dest) || (!src) || (!len)){
            return ERROR;
        }
        char *srcP = (char *) src;
        char *destP = (char *) dest;
        char *temp = (char *)malloc(sizeof(char ) * len);

        //Use of temp to avoid ovelapping addresses in memory between src and dest
        for (int cont=0; cont<len; cont++){
            temp[cont] = srcP[cont];
        }

        for (int cont=0; cont<len; cont++){
            destP[cont] = temp[cont];
        }
        free(temp);
        //free(srcP);

    return ERROR;
}

/**
 * Ex.5: Analyse une ligne de transition
 * @param line la ligne à lire
 * @param len la longueur de la ligne
 * @return la transition ou NULL en cas d'erreur
 */
transition *parse_line(char *line, size_t len) {
    int nbMotsParLigne = 0;
    char* etat;

    if (!line){
        printf(" args 1 de parse_line est NULL!\n");
        return NULL;
    }
    if (!len){
        printf(" args 2 parse_line est NULL!\n");
        return NULL;
    }

    transition* transit;
    int transitElemNumber=0;
    transit = malloc(len);//pas besoin de +1 car c'est le contenant des strings qui aurons besoin de +1

    if (!transit){
        printf("malloc n'a pas alloue mem a transit!");return NULL;
    }

    //divisons la lignes en mots utiles et Allouons les espaces memoires convenables
    char *lineCopy = line;
    int LineLen = strlen2(line);

    char *word;
    char *word1;
    char *word2;
    char *word3;
    char *word4;
    char *word5;

    int wordLen=0;
    int wordLen1=0;
    int wordLen2=0;
    int wordLen3=0;
    int wordLen4=0;
    int wordLen5=0;

    //On compte le nombre de caracteres valides de chaque mot etat pour savoir combien allouer de memoire
    //En passant on ramasse le pointeur du debut de chacun de ses mots
    for (int cont = 0; cont <=LineLen; cont++) {
        nbMotsParLigne = 0;

        //au cas la ligne commence par '(' sinon le while prochain serait
        // capable de traiter les autres cas meme pour un seul caractere
        if (*lineCopy == '(') {
            lineCopy++;
        }
        while ((*lineCopy != '(') && (*lineCopy != ')') && (*lineCopy != '-') && (*lineCopy != '>') &&
               (*lineCopy != ',') && (*lineCopy) &&(*lineCopy != '\0') && (*lineCopy !='\r') && (*lineCopy !='\n')){

            printf("lineCopy avant= %c \n", *lineCopy);
            if (wordLen==0){
                //on ramasse le debut des mots
                word =lineCopy;
            }
            wordLen++;
            lineCopy++;//on avance dans la ligne
            printf("lineCopy apres= %c \n", *lineCopy);
            printf("wordLen = %d \n", wordLen);
        }
        if (wordLen != 0) {
            transitElemNumber++;
            switch (transitElemNumber) {
                case 1:
                    wordLen1 = wordLen;
                    word1 = word;
                    printf("wordLen1 = %d \n", wordLen1);
                    printf("word1 = %c \n", *word);
                    break;
                case 2:
                    wordLen2 = wordLen;
                    word2 = word;
                    printf("wordLen2 = %d \n", wordLen2);
                    printf("word2 = %c \n", *word);
                    break;
                case 3:
                    wordLen3 = wordLen;
                    word3 = word;
                    printf("wordLen3 = %d \n", wordLen3);
                    printf("word3 = %c \n", *word);
                    break;
                case 4:
                    wordLen4 = wordLen;
                    word4 = word;
                    printf("wordLen4 = %d \n", wordLen4);
                    printf("word4 = %c \n", *word);
                    break;
                case 5:
                    wordLen5 = wordLen;
                    word5 = word;
                    printf("wordLen5 = %d \n", wordLen5);
                    printf("word5 = %c \n", *word);
                    break;
            }
            wordLen = 0;
        }
        if((*lineCopy !='\r')&&(*lineCopy !='\n')&&(*lineCopy != '\0') &&(*lineCopy != -1) && (*lineCopy != EOF)){
            lineCopy++;
        }
    }

    //on a ramasse la longueur des mots ainsi que l'adresse de leur debut dans la ligne par consequence
    // on alloue les memoires aux ele du struct, et on y copie les caracteres correspondants.
    //Mais vue que cette fonction n’analyse que les lignes qui représentent des transitions
    //verifions le nb de mot avant l'allocation de memoire

    if (wordLen1>0){nbMotsParLigne++;}
    if (wordLen2>0){nbMotsParLigne++;}
    if (wordLen3>0){nbMotsParLigne++;}
    if (wordLen4>0){nbMotsParLigne++;}
    if (wordLen5>0){nbMotsParLigne++;}
    printf("nbMotsParLigne = %d\n",nbMotsParLigne);

    if ((wordLen1>0) && (wordLen2<=0) && (wordLen3<=0) && (wordLen4<=0) && (wordLen5<=0)){
        etat = (char*) malloc(wordLen1+1);
        if(!etat){
            printf("No memory allocated to etat");
        } else {
            for(int letter = 0; letter <= wordLen1; letter++ ){
                etat[letter] = *word1;
                word1++;
                printf("etat = %c\n",etat[letter]);
            }
            etat[wordLen1] = '\0';
            printf("etat = %c\n",etat[wordLen1]);
        }
    }

    //Selon les instructions cette fonction fonctionnera avec un nombre de mots >=3
    if(nbMotsParLigne<3){
        printf("nombre de mots dans cette expression = %d < 3\n", nbMotsParLigne);
    }

    //Commencons l'allocation de la memoire et le remplissage des donnees dans les champs de la transition
    else if(nbMotsParLigne>=3) {
        if (wordLen1 > 0) {
            transit->current_state = (char *) malloc(wordLen1 + 1);
            if (!transit->current_state) {
                return NULL;
            } else{ //si la memoire etait allouee correctement
                for (int letter = 0; letter < wordLen1; letter++){
                    transit->current_state[letter] = *word1;
                    word1++;
                    printf("transit->current_state = %c\n", transit->current_state[letter]);
                }
                transit->current_state[wordLen1] = '\0';
                printf("transit->current_state = %c\n", transit->current_state[wordLen1]);
            }
        } else{
            printf("No state available for current state");
        }

        //Le client nous garantie que la longueur est un seul char correspondant au type du read ->char
        if (wordLen2 > 0){
            transit->read = *word2;
            printf("transit->read = %c\n", transit->read);
        } else{
            printf("No state available for read");
        }
        if (wordLen3 > 0) {
            transit->next_state = (char *) malloc(wordLen3 + 1);
            if (!transit->next_state) {
                printf("No memory allocated to next_state");
                return NULL;
            } else{ //si la memoire etait allouee correctement
                for (int letter = 0; letter < wordLen3; letter++) {
                    transit->next_state[letter] = *word3;
                    word3++;
                    printf("transit->next_state = %c\n", transit->next_state[letter]);
                }
                transit->next_state[wordLen3] = '\0';
                printf("transit->next_state = %c\n", transit->next_state[wordLen3]);
            }
        } else { printf("No state available for next_state"); }

        //Le client nous garantie que la longueur est un seul char correspondant au type du write ->char
        if (wordLen4 > 0){
            transit->write = *word4;
            printf("transit->write = %c\n", transit->write);
        } else{
            printf("No state available for write");
        }

        //Le client nous garantie que la longueur est un seul char correspondant au type du movement ->char
        if (wordLen5 > 0){
            transit->movement = *word5;
            printf("transit->movement = %c\n", transit->movement);
        } else{
            printf("No state available for movement");
        }

        //retournons la transition
        return transit;
    }
    return 0;
}

/**
 * Ex.6: Execute la machine de turing dont la description est fournie
 * @param machine_file le fichier de la description
 * @param input la chaîne d'entrée de la machine de turing
 * @return le code d'erreur
 */
error_code execute(char *machine_file, char *input) {
    int nombreEtat = 0;
    char* initialEtat;
    char* acceptantEtat;
    char* rejetantEtat;
    size_t maxLenLineG;
    char* etat;
    int nbMotsParLigne = 0;

    transition * transit;
    char *out =NULL;//the pointer to the line read from the file

    FILE *readFile = fopen (machine_file, "r");
    size_t nbLines = no_of_lines(readFile);

    //declaration of an array of pointers named description sized nbLines, by the way description is a pointer too
    transition * description[nbLines];

    for(int lineNb = 0; lineNb < nbLines; lineNb++) {
        readline( readFile, &out, maxLenLineG);
        transit = parse_line(out, strlen2( out)) ;

        //si c'est une ligne d'etat ramassons la .
        if ((nbMotsParLigne<3)&&(etat)){
            nombreEtat++;
            switch (nombreEtat) {
                case 1:
                    initialEtat = etat;
                    printf("initialEtat = %c \n", *etat);
                    break;
                case 2:
                    acceptantEtat = etat;
                    printf("acceptantEtat = %c \n", *etat);
                    break;
                case 3:
                    rejetantEtat = etat;
                    printf("rejetantEtat = %c \n", *etat);
                    break;
            }
        }

        if(transit){
            description[nbLines] = transit;

            printf("1transit->current_state= %c\n", *description[nbLines]->current_state);
            printf("1transit->read= %c\n", description[nbLines]->read);
            printf("1transit->next_state = %c\n", *description[nbLines]->next_state);
            printf("1transit->write = %c\n", description[nbLines]->write);
            printf("1transit->movement= %c\n", description[nbLines]->movement);
        }

        //on ramasse la position courante
        long int posInitial = ftell(readFile);

        //on remet le curseur a la pos init
        fseek(readFile, 0, SEEK_SET);
        fseek(readFile, posInitial, SEEK_SET);
    }

    char* ruban;
    int lenInput = strlen2(input);
    ruban = (char *) malloc(lenInput);
    memcpy2(ruban, input, lenInput);

    char* teteLecture;
    teteLecture = ruban;

    for(int lineNb = 0; lineNb < nbLines; lineNb++){
        if(nombreEtat == 2){
            return HAS_NO_ERROR(1);
        }
        if(nombreEtat == 3){
            return HAS_ERROR(0);
        } else {
            teteLecture = fgetc(machine_file);
            execute(ruban, teteLecture);
        }

    }
}

// ATTENTION! TOUT CE QUI EST ENTRE LES BALISES ༽つ۞﹏۞༼つ SERA ENLEVÉ! N'AJOUTEZ PAS D'AUTRES ༽つ۞﹏۞༼つ

// ༽つ۞﹏۞༼つ

int main() {
// ous pouvez ajouter des tests pour les fonctions ici
    FILE *fp;
    fp = fopen("../five_lines", "r");
    int nb = no_of_lines(fp);
    printf("%d", nb);
    return 0;
}

// ༽つ۞﹏۞༼つ