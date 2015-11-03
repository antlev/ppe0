/*
*   PPE : Programme de gestion d'adresses IP V4
*   Version : v1.7
*   Auteur :Antoine Levy SIO2
*
*   Notes : Pour compiler ce programme, il faut rajouter l'option -lm
*   exemple : gcc sisrv1.7.c -o sisrv1.7 -lm
*
*   TODO : - Calcul de la derniere adresse ip
*          - Calcul du prochain réseau
*/
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <math.h>
//*********************-Procédures-*********************
void sayHello(); //  Dis Bonjour
void sayGoodBye(); // Dis Au-revoir
void error(int code); // Affiche les messages d'error du programme
void saveAdress(char char100[], int *decInt4,int *binInt32, int ipOrMask); // Enregistre l'adresse saisie dans char100 dans decInt4 et binInt32
void saveNetworkAdresse(int ipBinInt32[],int ipDecInt4[],int maskBinInt32[],int maskDecInt4[], int *networkBinInt32, int *networkDecInt4); // Enregistre l'adresse réseau dans dec
void printAdress4(int decInt4[]); // Affiche une adresse en decimal pointé
void printAdress32(int binInt32[]); // Affiche une adresse en binaire
void base10ToBinaire(int n, int *binInt8); // Prend une valeur n compris entre 0 et 255 et la stock sous forme binaire dans un tableau de 8 cases
void calcNextAdress(int networkBinInt32[],int *firstAdress4); // TODO Rempli un tableau decPoint4 de la première adresse ip de la plage
void calcPreviousAdress(int networkBinInt32[],int *lastAdress4); // TODO Rempli un tableau decPoint4 de la dernière adresse ip de la plage
void calcNextNetwork(int networkBinInt32[],int maskBinInt32[],int *nextNetworkBinInt32); // Prend en entrée l'adresse reseau et le masque et rempli les tableaux nextNetworkDecInt4 et nextNetworkBinInt32
void convAdressBinToDec(int binInt32[],int *decInt4); // Convertit une adresse 32 bits en decimal pointé
void Ad32To4(int binInt32[],int *decInt4); // Todo Transforme une adresse 32 bits en decimal pointe
void Ad4To32(int decInt4[],int *binInt32); // Todo Transforme une adresse  en decimal pointe en 32 bits
//*********************-Fonctions-*********************
int checkAdress(char char100[],int ipOrMask); // Vérifie la validité de l'adresse
int checkIfAdressExist(int *decInt4, int ipOrMask); // Check si une adresse (ip ou masque) existe en mémoire
int binaireToBase10(int binInt[],int bit); // Prend en entrée une chaine de caractère binaire, allant jusqu'à 32bit et renvoi la valeur décimale
int calcRange(int maskBinInt32[]); // Calcul le nombre d'ip disponible en fonction du masque
struct SousReseau{
    int adresse[32];
    int masquecidr;
}
//*********************-Main-*********************
main(){
    int i,j,k,n,numberInt,maskcidr,choice,valToConv,nbSousRes,nbAdresses;
    int ipDecInt4[4],maskDecInt4[4],networkDecInt4[4],ipBinInt32[32],maskBinInt32[32],networkBinInt32[32],tabBinInt8[8],firstAdress4[4],lastAdress4[4],nextNetworkBinInt32[32],nextNetworkDecInt4[4],temp4[4],broadcastAdress4[4],nbAdressSousRes[32],rangeResBroadAd4[4] ;
    int masqueBin[32];
    int AdresseSousReseau[32][32];
    int rangeResBroadAd[4],rangeResNetworkAd[4];
    int rangeResNetworkAd4[4];
    int masquecidr,temp2,temp3;
    char ipChar100[100],masqueChar100[100],temp[4],binToDec32[32];
    char point=0; // Permet de verifié la structure de l'adresse ip donnée
    for( j=0 ; j<4 ; j++ ) // Initialisation des tableaux ip et masque à 0;
    {
        ipDecInt4[j] = maskDecInt4[j] = 0;
    }
    for ( j=0 ; j<32 ; j++)
    {
        ipBinInt32[j] = maskBinInt32[j] = 0 ;
    }
    sayHello();
    while (i != 6)
    {
        printf("\n");
        printf("***************** MENU PRINCIPAL *****************\n");
        printf("Entrez une valeur correspondant a votre choix :\n");
        printf("1) Saisir une adresse ip en mémoire\n");
        printf("2) Saisir un masque de sous réseau en mémoire\n");
        printf("---------------------------------------------------\n");
        printf("3) Info réseau actuellement en mémoire\n");
        printf("4) Conversion\n");
        printf("5) Sous-réseau\n");
        printf("6) Quitter\n");
        printf("**************************************************\n");
        if ( checkIfAdressExist(ipDecInt4,0) == 1 )
        {
            printf("ip actuellement en mémoire :     ");
            printAdress4(ipDecInt4);
        } else printf("Aucune adresse ip n'est actuellement en mémoire\n");
        if ( checkIfAdressExist(maskDecInt4,1) == 1 )
        {
            printf("Masque actuellement en mémoire : ");
            printAdress4(maskDecInt4) ;
        } else printf("Aucun masque n'est actuellement en mémoire\n");
        if ( checkIfAdressExist(ipDecInt4,0) == 1 && checkIfAdressExist(maskDecInt4,1) == 1 )
        { 
            saveNetworkAdresse(ipBinInt32,ipDecInt4,maskBinInt32,maskDecInt4,networkBinInt32,networkDecInt4);
            printf("Adresse réseau calculée :        ");
            printAdress4(networkDecInt4);
        }
        printf("**************************************************\n");
        scanf("%d",&i);
        switch (i){
        case 1 :
        printf("************* Saisie de l'adresse IP *************\n");
        if ( checkIfAdressExist(ipDecInt4,0) == 1 )
        {
            printf("Adresse ip actuellement en mémoire\n");
            printAdress4(ipDecInt4);
        } else { printf("Aucune adresse ip n'est rentrée en mémoire\n");}
        do{ 
            scanf("%s",ipChar100); // On demande une ip que l'on stock dans char100
            n = checkAdress(ipChar100,0) ; // On vérifie la validité de l'ip
            if ( n == 1 )
            {
                printf("Adresse IP valide\n");
                saveAdress(ipChar100,ipDecInt4,ipBinInt32,0); // Si l'ip est valide on sauvegarde l'ip donné dans le tableau ipDecInt4
            }
        } while ( n  != 1);  // tant que l'ip n'est pas valide : reprompt
        break;
      
        case 2 :
        printf("Entrez 1 pour saisir un masque en format adresse\n(xxx.xxx.xxx.xxx)\n");
        printf("Entrez 2 pour saisir un masque en format cidr (/xx)\n");
        scanf("%d",&choice);
        switch (choice){
            case 1 :
            printf("******** Saisie du masque : Format Adresse ********\n");
            if ( checkIfAdressExist(maskDecInt4,1) == 1 )
            {
                printf("Masque actuellement en mémoire :\n");
                printAdress4(maskDecInt4) ;
            } else { printf("Aucun masque n'est actuellement rentré en mémoire\n") ; }
            printf("Veuillez saisir un masque de sous réseau dans \nun format xxx.xxx.xxx.xxx\n");
            do{ 
                scanf("%s",masqueChar100); // On demande un masque que l'on stock dans masqueChar100
                n = checkAdress(masqueChar100,1) ; // On vérifie la validité du masque
                if ( n == 1 )
                {
                    printf("Masque valide\n");
                    saveAdress(masqueChar100,maskDecInt4,maskBinInt32,1); // Si l'ip est valide on sauvegarde l'ip donné dans le tableau ipDecInt4
                }
            } while ( n  != 1);  // tant que l'ip n'est pas valide : reprompt
            break;
            case 2 :      
            printf("********* Saisie du masque : Format CIDR *********\n");
            if ( checkIfAdressExist(maskDecInt4,1) == 1 )
            {
                printf("Masque actuellement en mémoire :\n");
                printAdress4(maskDecInt4) ;
            }
            else { printf("Aucun masque n'est actuellement rentré en mémoire\n") ; }
            printf("Veuillez entrer un masque en format CIDR\nC'est-à-dire un entier compris entre 0 et 32\n");
            scanf("%d",&maskcidr);
            if ( maskcidr <= 0 || maskcidr > 31 ) printf("La valeur cidr du masque doit être comprise entre 0 et 32\n");

            while ( maskcidr < 1 || maskcidr > 31 )
            {
                printf("Veuillez resaisir un masque en format cidr\n");
                scanf("%d",&maskcidr);
                if ( maskcidr <= 0 || maskcidr > 31 ) printf("La valeur cidr du masque doit être comprise entre 0 et 32\n");
            }
            if ( maskcidr == 31 ) printf("Note : Vous vous appretez à rentrer un masque ayant qu'un seul bit à 0\nLe réseau correspondant ne pourra comporter qu'une seul adresse ip\n");
            i = 0;
            while ( maskcidr != 0 )                       // maskcidr représente le nombre de 1 dans le masque
            {                                               //
               if (maskcidr >= 8 )                        // Tant que l'on a plus de 8 bit à 1 dans le masque
               {                                            //
                       maskDecInt4[i] = 255;         // => on remplis un octet de 1 soit 255 en décimal
                       for( k=i*8 ; k<i*8+8 ; k++ )
                       {
                            maskBinInt32[k] = 1 ;
                       }
                       maskcidr -= 8;                     // On retire les 8 bits à 1
                       i++;                                 // On avance d'un octet
               } else {                                     // Si il reste mois de 8 bits à 1
                       for ( j=0 ; j<maskcidr ; j++ )     // on rentre 2⁷ pour le premier bit puis 2⁶ puis 2⁵ etc...
                       {
                           maskDecInt4[i] += pow(2,(7-j));
                           maskBinInt32[i*8+j] = 1 ;
                       }
                       maskcidr = 0 ;
                   }
            } // Message indiquant la sauvegarde en mémoire du masque
            printf("Sauvegarde du masque %d.%d.%d.%d en mémoire\n", maskDecInt4[0],maskDecInt4[1],maskDecInt4[2],maskDecInt4[3] ); 
            break;
            default :
            break;
        }
        break;               
        case 3 :
            if( checkIfAdressExist(ipDecInt4,0) == 1 && checkIfAdressExist(maskDecInt4,1) == 1)
            {
               
                printf("------------------- Info réseau -------------------\n");
                // CALCUL DES ADRESSES
                calcNextNetwork(networkBinInt32,maskBinInt32,nextNetworkBinInt32); // PROCHAIN RESEAU EN BINAIRE
                convAdressBinToDec(nextNetworkBinInt32,nextNetworkDecInt4); // CONVERSION PROCHAIN RESEAU EN DECIMAL POINTE
                calcNextAdress(networkDecInt4,firstAdress4); // CALCUL DE LA PREMIERE ADRESSE IP DISPONIBLE
                calcPreviousAdress(nextNetworkDecInt4,broadcastAdress4); // CALCUL DE L ADRESSE DE BROADCAST
                calcPreviousAdress(broadcastAdress4,lastAdress4); // CALCUL DE LA DERNIERE ADRESSE IP DISPONIBLE

                // AFFICHAGE DES ADRESSES
                printf("******************* En binaire *******************\n");
                printf("Adresse ip : ");
                for ( j=0 ; j<32 ; j++ )
                {
                    printf("%d", ipBinInt32[j]);
                    if ( j == 7 || j == 15 || j == 23 ) { printf("."); }
                }
                printf("\nMasque     : ");
                for ( j=0 ; j<32 ; j++ )
                {
                    printf("%d", maskBinInt32[j]);
                    if ( j == 7 || j == 15 || j == 23 ) { printf("."); }
                }
                printf("\nReseau     : ");
                printAdress32(networkBinInt32);
                printf("\n******************* En décimal *******************\n");
                printf("Adresse ip     : ");
                printAdress4(ipDecInt4);
                printf("Masque         : ");
                printAdress4(maskDecInt4);
                printf("Adresse réseau : ");
                printAdress4(networkDecInt4);
                printf("\n1ère adresse ip disponible       : ");
                printAdress4(firstAdress4);
                printf("Nombres d'adresses ip disponible : %d\n",calcRange(maskBinInt32));             
                printf("Dernière adresse ip disponible   : ");
                printAdress4(lastAdress4);
                printf("Adresse de broadcast             : ");  
                printAdress4(broadcastAdress4);
                printf("Prochain réseau                  : ");   
                printAdress4(nextNetworkDecInt4);
                printf("----------------------------------------------------\n\n\n");
            } else {
                printf("Il faut une adresse ip et un masque valide pour afficher les infos du réseau\n");
                printf("Veuillez vérifier ces paramètres svp\n");
            }
        break;
        case 4 :
        choice = 0;
        while ( choice != 7 )
        {
        printf("******************* Conversion *******************\n");
        printf("1) Conversion d'un nombre décimal en binaire\n");
        printf("2) Conversion d'un nombre décimal en héxadécimal\n");
        printf("3) Conversion d'un nombre binaire en décimal\n");
        printf("4) Conversion d'un nombre binaire en héxadécimal\n");
        printf("5) Conversion d'un nombre héxadécimal en décimal\n");
        printf("6) Conversion d'un nombre héxadécimal en binaire\n");
        printf("7) Quitter Conversion\n");
        scanf("%d",&choice);
            switch (choice){
                case 1 :
                    printf("Quel est le nombre décimal à convertir en binaire?\n");
                    scanf("%d",&valToConv);
                    while ( valToConv > 255 || valToConv < 0 )
                    {
                        printf("Veuillez rentrer une valeur comprise entre 0 et 255\n");
                        scanf("%d",&valToConv);
                    }
                    printf("Votre valeur décimale : %d\n", valToConv);
                    printf("Conversion : " );
                    base10ToBinaire(valToConv,tabBinInt8);
                    for ( j=7 ; j>=0 ; j-- )
                    {
                        printf("%d",tabBinInt8[j]);
                    }
                    printf("\n");
                break;
                // REVOIR CES 3 POINTS
    /*            case 2 :
                    printf("Quel est le nombre décimal à convertir en héxadécimal?\n");
                    scanf("%d",&valToConv);
                    //printf("Votre valeur binaire : %d\n",binToDec32 );
                    printf("%d vaut %X en héxadécimal\n",valToConv,valToConv);
                break;
                case 3 :
                    printf("Quel est le nombre binaire à convertir en décimal?\n");
                    sscanf(binToDec32,"%d",&j);
                    temp3 = 0 ;
                    while ( binToDec32 >= 1 )
                    {
                        binToDec32 /= 10 ;
                        temp3++;
                    }
                    printf("Votre valeur en décimal : %d\n",binaireToBase10(binToDec32,temp3));
                break;
                case 4 :
                    printf("Quel est le nombre binaire à convertir en héxadécimal?\n");
                    scanf("%s",binToDec32);
                //    fgets(binToDec32,32,stdin);
                    temp3 = 0 ;
                    while ( binToDec32 >= 1 )
                    {
                        binToDec32 /= 10 ;
                        temp3++;
                    }
                    printf("Votre valeur en héxadécimal : %X\n",binaireToBase10(binToDec32,temp3));
                break; */
                case 5 :
                    printf("Quel est le nombre hexadécimal à convertir en décimal?\n");
                    scanf("%X",&valToConv);
                    printf("%X vaut %d en décimal\n",valToConv,valToConv );
                break;
                case 6 :
                    printf("Quel est le nombre hexadécimal à convertir en binaire?\n");
                    scanf("%X",&valToConv);
                    while ( valToConv > 255 || valToConv < 0 )
                    {
                        printf("Veuillez rentrer une valeur comprise entre 0 et 255\n");
                        scanf("%X",&valToConv);
                    }
                    base10ToBinaire(valToConv,tabBinInt8);
                    for ( j=0 ; j<8 ; j++ )
                    {
                        printf("%d",tabBinInt8[j]);
                    }
                    printf("\n");
                    break;
                case 7 :
                printf("Quitter Conversion\n");
                break;
                default :
                break;
            }
        }      
        break;
        case 5 :
        {
            struct SousReseau Adresse[100];
            for ( j = 0 ; j < 32 ; j++ )
            {
                masqueBin[j] = 1 ;
            }
            int nbAdressesTot,nbAdressesPossible ;
            nbAdressesTot = 0 ;
            if( checkIfAdressExist(ipDecInt4,0) == 1 && checkIfAdressExist(maskDecInt4,1) == 1)
            {
                for ( j = 0 ; j < 4 ; j++ )
                {
                    rangeResBroadAd4[j] = networkDecInt4[j] ;
                }

                printf("******************** SOUS-RESEAU *******************\n");
                printf("Combien de sous-réseaux souhaitez vous créer ?\n");
                scanf("%d",&nbSousRes);
                printf("Veuillez rentrer les sous réseau souhaité du PLUS GRAND au PLUS PETIT\n");
                for( j = 0 ; j < nbSousRes ; j++ )
                {
                    printf("Combien d'adresses doit contenir le sous-réseau n° %d ?\n",j+1 );
                    scanf("%d",&nbAdresses);
                    nbAdressSousRes[j] = nbAdresses ;
                    nbAdressesTot += nbAdresses ;
                }
                printf("sort\n");
                for( j = 31 ; j >= 0 ; j-- )
                {
                    if ( maskBinInt32[j] == 0 )
                    {
                        temp2++ ;
                    } else break ;
                }
                nbAdressesPossible = pow(2,temp2);
                if ( nbAdressesPossible < nbAdressesTot + 2 * nbSousRes )
                {
                    printf("Vous ne pouvez pas rentrer autant d'adresses étant donnée votre plage\n");
                    printf("Votre plage vous permet de dispser de %d \n",nbAdressesPossible );
                    printf("Rappel : chaque sous-réseau nécessite de deux adresses supplémentaire (réseau et broadcast)\n");
                } else
                {  
                    masquecidr = 32;
                    for ( j = 0 ; j < nbSousRes ; j++ )
                    {
                        while ( nbAdressSousRes[j] > 1 )                       
                        {
                                nbAdressSousRes[j] /= 2 ;
                                masquecidr-- ; 
                        }
                        Adresse[j].masquecidr = masquecidr ;
                        for ( k = 32 ; k >= masquecidr ; k-- )
                        {
                            masqueBin[k] = 0;
                            printf("%d",masqueBin[k] );
                        }

                        //Calcul de la plage réservée
                        printf("7\n");
                       
                        calcNextAdress(rangeResBroadAd,rangeResNetworkAd);
                        calcNextNetwork(rangeResNetworkAd,masqueBin,rangeResBroadAd);

                        printAdress4(rangeResNetworkAd);
                        printAdress4(rangeResBroadAd);


                    }
                    printf("9\n");

                }

            } else { printf("Il faut avoir en mémoire une adresse ip et un masque valide pour avoir cette option\nVeuillez vérifier ces paramètrs svp\n"); }
        }
        break;
        case 6 :
            printf("********************* SORTIE *********************\n");
            printf("Voulez vous vraiment quitter mon programme?? ;(\n");        
            printf("( 6 ) pour confirmer ;(\n");
            printf("**************************************************\n");
            scanf("%d",&i);
            if(i==6)
            {
                sayGoodBye();
            } else break;
        return;
    
        default :
        break;
        }
    }
}
//*********************-Procédures-*********************
void sayHello(){
    printf("\n");
    printf("**************************************************\n");
    printf("Bonjour et bienvenue sur mon programme\n");
    printf("**************************************************\n");
}
void sayGoodBye(){
    printf("\n");
    printf("**************************************************\n");
    printf("Au revoir et à bientôt :)\n");
    printf("**************************************************\n");
}
void error(int code){
    switch(code){
        //***** ip et Masque *****//
        case 1 :
            printf("ERREUR 1 : Format non respecté \n")    ; // Message d'error si la forme xxx.xxx.xxx.xxx n'est pas respectée
            printf("RAPPEL : L'adresse saisie doit respecter le format : xxx.xxx.xxx.xxx\n");
            printf("Les valeurs 'xxx' doivent être comprises entre 0 et 255\n\n");
        break;
        case 2 :
            printf("ERREUR 2 : Valeur hors-limite \n")    ; // Message d'error si une valeur xxx est hors limite
            printf("RAPPEL : L'adresse saisie doit respecter le format : xxx.xxx.xxx.xxx\n");
            printf("Les valeurs 'xxx' doivent être comprises entre 0 et 255\n\n");
        break;
        //***** Masque *****//
        case 3 :
            printf("ERREUR 3 : Masque saisie invalide ! : Bits à 1 non contigus\n");
            printf("RAPPEL : L'adresse saisie doit respecter le format : xxx.xxx.xxx.xxx\n");
            printf("Les bits à 1 doivent être contigus\n");
        break;
    }
}
int checkIfAdressExist(int *decInt4,int ipOrMask) // Check si une adresse (ip ou masque) existe en mémoire
{
    int j;
    for ( j=0 ; j<4 ; j++ ) // On regarde si une adresse existe en mémoire
    {   // Si oui on renvoie 1
        if ( decInt4[j] != 0 ) { return 1 ; }
        else if ( j == 3 ) { return 0 ; }         
    }
}
void saveAdress(char char100[], int *decInt4,int *binInt32, int ipOrMask) // Enregistre l'adresse saisie dans char100 dans decInt4 et binInt32
{
    int j,k,a=0,numberInt ;
    int binInt8[8];
    char temp[3];
    char point=0; // Permet de verifier la structure de l'adresse ip donnée
    
    printf("%s\n",char100 );

    for( j=0 ; j <= strlen(char100) ; j++ ) // On parcourt le tableau...
    {
            if( char100[j] != '.' && char100[j] != '\0' ) // ... si on ne trouve pas de point ni la fin du tableau
            {
                temp[a] = char100[j]    ;    // on stock la valeur de l'char100 de j dans une vaariable temp
            }
            else if ( ( char100[j] == '.' || char100[j] == '\0' ) && point < 4 ) // ... si on trouve un point ou la fin du tableau
            {
                numberInt = atoi(temp)    ; // on ressort la valeur numérique contenue dans temp
                temp[0]=0;
                a=-1; // On réinitialise temp et son compteur
                temp[1]=temp[2]='\0';
                if ( numberInt >= 0 && numberInt < 256)  // On vérifie que la valeur contenue maintenant dans
                {                                        // numberInt ne dépasse pas les limites
                    decInt4[point] = numberInt;
                    base10ToBinaire(numberInt,binInt8);
                    for ( k=0 ; k<8 ; k++ )
                    {
                        binInt32[point*8+k] = binInt8[7-k];
                    }
                    point++;
                }
            }
            a++; // a permet d'avoir un compteur pour le tableau temp    
    }
    if ( ipOrMask == 0 )
    {
        printf("Sauvegarde de l'adresse ip %d.%d.%d.%d en mémoire \n",decInt4[0],decInt4[1],decInt4[2],decInt4[3]);
    }
    if ( ipOrMask == 1 )
    {
        printf("Sauvegarde du masque %d.%d.%d.%d en mémoire \n",decInt4[0],decInt4[1],decInt4[2],decInt4[3]);
    }
}
void saveNetworkAdresse(int ipBinInt32[],int ipDecInt4[],int maskBinInt32[],int maskDecInt4[], int *networkBinInt32, int *networkDecInt4)
{
    int j;
    for ( j=0 ; j<32 ; j++ )
    {
        networkBinInt32[j] = ipBinInt32[j] & maskBinInt32[j] ;
    }
    for ( j=0 ; j<4 ; j++ )
    {
       networkDecInt4[j] = ipDecInt4[j] & maskDecInt4[j] ;  // Et Logique entre l'adresse ip et le masque =  adresse reseau
    }
}
void printAdress4(int decInt4[])
{
    printf("%d.%d.%d.%d\n", decInt4[0],decInt4[1],decInt4[2],decInt4[3]);
         
}
void printAdress32(int binInt32[])
{
    int j;
    for ( j=0 ; j<32 ; j++ )
    {
        printf("%d",binInt32[j] );
        if ( j==7 || j==15 || j==23 ) { printf("." ) ; }
    }
    printf("\n");
}  
void base10ToBinaire(int n, int *binInt8) // Prend une valeur n compris entre 0 et 255 et la stock sous forme binaire dans un tableau de 8 cases
{
    int j;
    for ( j=0 ; j<8 ; j++ )
    {
        binInt8[j] = n%2 ;
        n = n/2 ;
    }
}
void calcNextAdress(int baseDecInt4[],int *nextAdress4) // TODO Rempli un tableau decPoint4 de la première adresse ip de la plage
{
    int j;
    for( j=0 ; j<4 ; j++ )
    {
        nextAdress4[j] = baseDecInt4[j]; // On rentre l'adresse réseau dans nextAdress4
    }

    for ( j=3 ; j>=0 ; j-- ) // On ajoute 1 pour avoir la première adresse ip disponible
    {
        if ( nextAdress4[j] < 255)
        {
            nextAdress4[j] += 1;
            break;
        } else if ( nextAdress4[j] == 255)
        {
            nextAdress4[j] = 0;
        }
    }
}
void calcPreviousAdress(int baseDecInt4[],int *previousAdress4) // TODO Rempli un tableau decPoint4 de la dernière adresse ip de la plage
{
    int j;
    for( j=0 ; j<4 ; j++ )
    {
        previousAdress4[j] = baseDecInt4[j]; // On rentre l'adresse réseau dans previousAdress4
    }

    for ( j=3 ; j>=0 ; j-- ) // On ajoute 1 pour avoir la première adresse ip disponible
    {
        if ( previousAdress4[j] > 0)
        {
            previousAdress4[j] -= 1;
            break;
        } else if ( previousAdress4[j] == 0)
        {
            previousAdress4[j] = 255;
        }
    }   
}
void calcNextNetwork(int networkBinInt32[],int maskBinInt32[],int *nextNetworkBinInt32) // Prend en entrée l'adresse reseau et le masque et rempli les tableaux nextNetworkDecInt4 et nextNetworkBinInt32
{
    // ATTENTION FONCTION NE REMPLISSANT BUEN QUE LE TABLEAU EN BINAIRE
    // POURL INSTANT JE CONVERTIS LE TABLEAU AVEC UNE AUTRE FONCTION
    int temp=0,j,octet,pas;
    for( j=0 ; j<32 ; j++ )
    {
        nextNetworkBinInt32[j] = networkBinInt32[j];
    }  
    for( j=0 ; j<32 ; j++ )
    {
        if(maskBinInt32[j] == 1)
        {
            temp++;
        } else { break; }
    }
    temp--;
    for ( j=temp ; j>=0 ; j-- )
    {
        if ( nextNetworkBinInt32[j] == 0 )
        {
            nextNetworkBinInt32[j] = 1;
            break;
        } else {
            nextNetworkBinInt32[j] = 0 ;
        }
    } 
  /*  printAdress32(networkBinInt32);
    printAdress32(maskBinInt32);
    printAdress32(nextNetworkBinInt32);
   for( j=0 ; j<32 ; j++ )
    {
        temp += pow(2,7-j%8);
        if ( j == 7 || j == 15 || j == 23 || j == 31 )
        {
            nextNetworkDecInt4[j%8] = temp;
            printf("test : %d",nextNetworkDecInt4[j%8]);
            temp = 0 ;
        }
    }*/
}
void convAdressBinToDec(int binInt32[],int *decInt4) // Convertit une adresse 32 bits en decimal pointé
{
    int i=0,j,temp=0;
    for ( j = 0; j < 4; ++j)
    {
        decInt4[j] = 0 ;
    }
    for ( j = 0; j < 32; ++j)
    {
        if ( binInt32[j] == 1 )
        {
            temp += pow(2,7-j%8);
        }

        if ( j == 7 || j == 15 || j == 23 )
        {
            decInt4[i] = temp ;
            temp = 0 ;
            i++;
        }
    }
}
void Ad32To4(int binInt32[],int *decInt4) // Transforme une adresse 32 bits en decimal pointe
{
    // WORKS!
    int j,k;
    int temp[8]; // Permet d'isoler un octet et de transformer sa valeu en decimal grace a la fonction binaireToBase10
    for ( j = 0 ; j < 4 ; j++ )
    {
        for ( k = 0 ; k < 8 ; k++ )
        {
            temp[k] = binInt32[j*8+k];
        }
        decInt4[j] = binaireToBase10(temp,8);
    }
}
void Ad4To32(int decInt4[],int *binInt32) // Transforme une adresse en decimal pointe en adresse 32 bits
{
    // WORKS!
    int binInt8[8];
    int j,k,n;
    int temp[8]; // Permet d'isoler un octet et de transformer sa valeu en decimal grace a la fonction binaireToBase10
    base10ToBinaire(192,binInt8);
    for ( j = 0 ; j < 4 ; j++ )
    {
        n = decInt4[j] ;
        base10ToBinaire(n,binInt8);
        for ( k = 0 ; k < 8 ; k++ )
        {
            binInt32[j*8+k] = binInt8[k];
        }
        printf("\n");
    }
}
//*********************-Fonctions-*********************
int checkAdress(char char100[], int ipOrMask){ // Vérifie la validité de l'adresse
    int j,a=0,numberInt,compteur,octetOk,coupure=0 ;           // est valide : renvoie 1 si valide, 0 sinon
    char temp[3];
    char point=0; // Permet de verifier la structure de l'adresse ip donnée
    int longueur = strlen(char100);
    for( j=0 ; j <= longueur ; j++ ) // On parcourt le tableau...
    {
            if( char100[j] != '.' && char100[j] != '\0' ) // ... si on ne trouve pas de point ni la fin du tableau
            {
                temp[a] = char100[j]    ;    // on stock la valeur de l'char100 de j dans une vaariable temp
            }
            else if ( ( char100[j] == '.' || char100[j] == '\0' ) && point < 4 ) // ... si on trouve un point ou la fin du tableau
            {

                if( char100[j] == '.' && char100[j+1] == '.'){ // Empêche la saisie de deux points d'affilée
                    error(1);
                    return 0;
                }
                if( char100[j] == '.' && char100[j+1] == '\0'){ // Empêche la saisie d'un point en dernier caractère
                    error(1);
                    return 0;
                }
                numberInt = atoi(temp)    ; // on ressort la valeur numérique contenue dans temp
                if ( coupure == 1 && numberInt != 0 )
                {
                    error(3);
                    printf("Veuillez resaisir un masque valide svp\n");
                    return 0 ;
                }
                if ( ipOrMask == 1 ){
                    if ( numberInt == 255 ) { compteur++; } else { coupure = 1 ; } // Marque la coupure entre les 1 et les 0
                    if ( compteur == 4 )  // Permet de refuser l'adresse 255.255.255.255
                    {
                        printf("Vous ne pouvez pas rentrer cette adresse : 255.255.255.255\n");
                        printf("Veuillez saisir un masque valide svp\n");
                        return 0;
                    }
                    int var = 128 ;
                    while ( var != 0 )
                    {
                        if (numberInt == 0) { var=0;} // Dans le cas ou l'octet vaut 0
                        if (numberInt == 255) { var=0; numberInt = 0;} // Dans le cas ou l'octet vaut 255
                        numberInt = numberInt - var ;
                        var = var / 2 ;
                        if(var == numberInt)
                        {
                            numberInt = 0 ;
                            var = 0 ;
                            octetOk++; // Valide un octet
                            if(octetOk == 4) // Si les 4 Octets sont valides
                            {
                                return 1;
                            }
                        }
                        if(numberInt<0)
                        {
                            error(3); // error 3 bits non contigus
                            return 0;
                        }
                    }
                    if ( numberInt != 0 )
                    {
                        error(3); // error 3 bits non contigus
                        return 0;
                    }
                }
                temp[0]=0;
                a=-1; // On réinitialise temp et son compteur
                temp[1]=temp[2]='\0';
               if ( numberInt >= 0 && numberInt < 256)  // On vérifie que la valeur contenue maintenant dans
                {                                        // numberInt ne dépasse pas les limites
                    point++;
                }
                else {
                    error(2);
                    return 0;
                }
            }
            else {
                    error(1);
                    return 0;
            }
            a++; // a permet d'avoir un compteur pour le tableau temp    
    }
    if(point != 4)
    {
        error(1);
        return 0;
    }
    return 1;
}
int binaireToBase10(int binInt[],int bit)
{
    int j, decimal = 0;
    for(j=0; j<bit; j++)
    {
        if (binInt[j]== 1 || binInt[j]== 0 )
            decimal = decimal * 2 +  binInt[j] ;
    }
    return decimal;
}
int calcRange(int maskBinInt32[]) // TODO Calcul le nombre d'ip disponible en fonction du masque
{
    int j,bit=32,range;
    for (  j=0 ; j < 32 ; j++ )
    {
        if ( maskBinInt32[j] == 1 )
        {
                bit--;
        }
    }
    range = pow(2,bit)-2;
    return range;
}