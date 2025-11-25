/* travel_fixed_by_destination.c
   Travel booking program with flights & hotels matched to destination.
   - Flight lists and hotel lists now depend on chosen destination.
   - Keeps same file format for info.txt (16 fields).
   - Input validation improved a bit.
   - Checkout date computed correctly using month lengths & leap years.
   - Names must be single token (no spaces) to keep file format simple.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int d,m,y;
int d1,m1,yr1;

void destination();
void print_details();
void print_ticket(int [], int);
void input_batch(int,int);
int id_exists(int);
void flights(int);
void hotels();
int menu();

struct info {
    int age;
    int cost;
    int days;
    char hname[30];
    char destination[20];
    char name[30];
    char gender;
    float time;
    char fname[30];
    char shift[4]; /* room for "AM"/"PM" + '\0' */
} i;

/* utility: leap year and days in month */
int is_leap(int yr) {
    return (yr%4==0 && yr%100!=0) || (yr%400==0);
}
int days_in_month(int mon, int yr) {
    if (mon==1||mon==3||mon==5||mon==7||mon==8||mon==10||mon==12) return 31;
    if (mon==4||mon==6||mon==9||mon==11) return 30;
    return is_leap(yr) ? 29 : 28;
}

/* Print details (search by ID) */
void print_details() {
    system("cls");
    int id, no, flag = 0;
    printf("\n\t TRAVELLER DETAILS :\n");
    printf("Enter ID of the person you are searching for: ");
    if (scanf("%d", &id) != 1) { while(getchar()!='\n'); printf("Invalid input\n"); return; }
    while(getchar()!='\n');

    FILE *f = fopen("info.txt", "r");
    if (!f) {
        printf("\nNo records (info.txt not found).\nPress enter to continue ...");
        getchar();
        return;
    }

    while (1) {
        int r = fscanf(f, "%d %29s %d %c %d %19s %29s %f %3s %d %d %d %d %d %d %29s",
                       &no, i.name, &i.age, &i.gender, &i.cost,
                       i.destination, i.fname, &i.time, i.shift,
                       &d,&m,&y,&d1,&m1,&yr1, i.hname);
        if (r != 16) break;

        if (no == id) {
            printf("\n\n\tID : %d", no);
            printf("\n\tNAME : %s\t HOTEL NAME : %s ", i.name, i.hname);
            printf("\n\tAGE : %d\t CHECKIN : %02d/%02d/%04d ", i.age, d, m, y);
            printf("\n\tGENDER : %c\t CHECKOUT : %02d/%02d/%04d", i.gender, d1, m1, yr1);
            printf("\n\tDESTINATION : %s", i.destination);
            printf("\n\tFLIGHT NAME : %s", i.fname);
            printf("\n\tDEPARTURE TIME : %.2f %s", i.time, i.shift);
            flag = 1;
            break;
        }
    }
    fclose(f);

    if (!flag) printf("\nNo record found for ID %d.", id);
    printf("\nPress enter to continue ...");
    getchar();
}

/* Print tickets for array of IDs and compute total */
void print_ticket(int a[], int cnt) {
    system("cls");
    int z, no;
    int total = 0;
    FILE *f = fopen("info.txt", "r");
    if (!f) {
        printf("\nNo records (info.txt missing)\nPress enter ...");
        getchar();
        return;
    }
    for (z = 0; z < cnt; ++z) {
        int target = a[z];
        int found = 0;
        rewind(f);
        while (1) {
            int r = fscanf(f, "%d %29s %d %c %d %19s %29s %f %3s %d %d %d %d %d %d %29s",
                           &no,
                           i.name,
                           &i.age,
                           &i.gender,
                           &i.cost,
                           i.destination,
                           i.fname,
                           &i.time,
                           i.shift,
                           &d,&m,&y,&d1,&m1,&yr1,
                           i.hname);
            if (r != 16) break;
            if (no == target) {
                printf("\n--------------------------\n");
                printf("ID : %d\nNAME : %s\nAGE : %d\nGENDER : %c\nHOTEL : %s\nCHECKIN: %02d/%02d/%04d\nCHECKOUT: %02d/%02d/%04d\nDEST: %s\nFLIGHT : %s\nDEPART TIME: %.2f %s\nCOST : %d\n",
                       no, i.name, i.age, i.gender, i.hname, d, m, y, d1, m1, yr1, i.destination, i.fname, i.time, i.shift, i.cost);
                total += i.cost;
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("\nID %d: not found in file (maybe not saved correctly).\n", target);
        }
    }
    fclose(f);
    printf("\n--------------------------\nTOTAL COST : %d\n", total);
    printf("\nBOOKING SUCCESSFUL. Press enter to go to main menu ...");
    getchar();
}

/* Check if id exists by scanning info.txt */
int id_exists(int id) {
    FILE *f = fopen("info.txt", "r");
    if (!f) return 0; /* no file => id not present */
    int no;
    while (1) {
        int r = fscanf(f, "%d %29s %d %c %d %19s %29s %f %3s %d %d %d %d %d %d %29s",
                       &no,
                       i.name, &i.age, &i.gender, &i.cost,
                       i.destination, i.fname, &i.time, i.shift,
                       &d,&m,&y,&d1,&m1,&yr1, i.hname);
        if (r != 16) break;
        if (no == id) { fclose(f); return 1; }
    }
    fclose(f);
    return 0;
}

/* Input batch of people for chosen destination */
void input_batch(int cnt, int cno) {
    if (cnt <= 0) {
        printf("Number of people must be > 0. Press enter ...");
        getchar();
        return;
    }

    int a[cnt];
    int x = 0;
    int id;

    /* set destination string */
    switch(cno) {
        case 1: strcpy(i.destination,"ITALY"); break;
        case 2: strcpy(i.destination,"FRANCE"); break;
        case 3: strcpy(i.destination,"LONDON"); break;
        case 4: strcpy(i.destination,"SYDNEY"); break;
        case 5: strcpy(i.destination,"NEW_YORK"); break;
        case 6: strcpy(i.destination,"TOKYO"); break;
        case 7: strcpy(i.destination,"PAKISTAN"); break;
        default: strcpy(i.destination,"UNKNOWN"); break;
    }

    while (x < cnt) {
        system("cls");
        printf("\n ENTER %d PERSON DETAILS (Destination: %s)\n", x+1, i.destination);

        /* choose ID and ensure uniqueness */
        do {
            printf("CHOOSE YOUR TRAVEL ID (1-10000): ");
            if (scanf("%d", &id) != 1) { while(getchar()!='\n'); id = -1; }
            while(getchar()!='\n');
            if (id < 1 || id > 10000) { printf("ID must be 1..10000\n"); id = -1; continue; }
            if (id_exists(id)) { printf("ID ALREADY EXISTS! CHOOSE ANOTHER ID.\n"); id = -1; }
        } while (id == -1);

        /* read simple name (no spaces) */
        printf("ENTER NAME (no spaces): ");
        scanf("%29s", i.name);
        while(getchar()!='\n');

        printf("ENTER AGE: ");
        if (scanf("%d", &i.age) != 1) { while(getchar()!='\n'); i.age = 0; }
        while(getchar()!='\n');

        printf("ENTER GENDER (M/F): ");
        scanf(" %c", &i.gender);
        while(getchar()!='\n');

        /* reset per-person fields */
        i.cost = 0;
        i.days = 0;
        i.hname[0] = '\0';
        i.fname[0] = '\0';
        i.time = 0.0f;
        i.shift[0] = '\0';

        /* choose flight and hotel (they update i.cost, etc) */
        flights(id);
        hotels();

        /* append to info.txt */
        FILE *f = fopen("info.txt", "a");
        if (!f) {
            printf("Error opening info.txt for writing.\nPress enter ...");
            getchar();
            return;
        }
        /* same format as reading: 16 fields */
        fprintf(f, "%d %s %d %c %d %s %s %.2f %s %d %d %d %d %d %d %s\n",
                id, i.name, i.age, i.gender, i.cost, i.destination, i.fname, i.time, i.shift,
                d,m,y,d1,m1,yr1, i.hname);
        fclose(f);

        a[x] = id;
        x++;
    }

    print_ticket(a, cnt);
}

/* hotels: choices tailored to destination */
void hotels() {
    int n;
    int base1, base2, base3;
    /* set hotel options depending on destination */
    if (strcmp(i.destination, "ITALY") == 0) {
        printf("\n HOTELS (Italy):\n1. FOUR_SEASONS_ROME 18000/DAY\n2. NH_COLLECTION 15000/DAY\n3. HOTEL_DE_RUSSIE 20000/DAY\n");
        base1 = 18000; base2 = 15000; base3 = 20000;
    } else if (strcmp(i.destination, "FRANCE") == 0) {
        printf("\n HOTELS (France):\n1. RITZ_PARIS 22000/DAY\n2. FOUR_SEASONS_PARIS 20000/DAY\n3. SOFITEL_PARIS 16000/DAY\n");
        base1 = 22000; base2 = 20000; base3 = 16000;
    } else if (strcmp(i.destination, "LONDON") == 0) {
        printf("\n HOTELS (London):\n1. RITZ_LONDON 23000/DAY\n2. SHANGRI_LA_LONDON 21000/DAY\n3. HILTON_PARK 17000/DAY\n");
        base1 = 23000; base2 = 21000; base3 = 17000;
    } else if (strcmp(i.destination, "SYDNEY") == 0) {
        printf("\n HOTELS (Sydney):\n1. PARK_HYATT_SYDNEY 19000/DAY\n2. SHANGRI_LA_SYDNEY 18000/DAY\n3. NOVOTEL_SYDNEY 14000/DAY\n");
        base1 = 19000; base2 = 18000; base3 = 14000;
    } else if (strcmp(i.destination, "NEW_YORK") == 0) {
        printf("\n HOTELS (New York):\n1. FOUR_SEASONS_NY 26000/DAY\n2. MANDARIN_ORIENTAL_NY 24000/DAY\n3. HYATT_CENTRAL_NY 18000/DAY\n");
        base1 = 26000; base2 = 24000; base3 = 18000;
    } else if (strcmp(i.destination, "TOKYO") == 0) {
        printf("\n HOTELS (Tokyo):\n1. PARK_HYATT_TOKYO 22000/DAY\n2. AMAN_TOKYO 25000/DAY\n3. HOTEL_GRAND_PACIFIC 15000/DAY\n");
        base1 = 22000; base2 = 25000; base3 = 15000;
    } else if (strcmp(i.destination, "PAKISTAN") == 0) {
        printf("\n HOTELS (Pakistan):\n1. MARRIOTT_ISB 12000/DAY\n2. AURA_LAHORE 9000/DAY\n3. PEARL_CONTINENTAL 11000/DAY\n");
        base1 = 12000; base2 = 9000; base3 = 11000;
    } else {
        printf("\n HOTELS (General):\n1. INTERNATIONAL_GRAND 17000/DAY\n2. CITY_HOTEL 15000/DAY\n3. ECONO_INN 9000/DAY\n");
        base1 = 17000; base2 = 15000; base3 = 9000;
    }

    printf("ENTER CHOICE (1-3): ");
    if (scanf("%d", &n) != 1) { while(getchar()!='\n'); printf("Invalid\n"); return; }
    while(getchar()!='\n');

    printf("ENTER NO OF DAYS: ");
    if (scanf("%d", &i.days) != 1) { while(getchar()!='\n'); i.days = 1; }
    while(getchar()!='\n');

    if (n == 1) {
        i.cost += i.days * base1;
        if (strcmp(i.destination, "ITALY") == 0) strcpy(i.hname,"FOUR_SEASONS_ROME");
        else if (strcmp(i.destination, "FRANCE") == 0) strcpy(i.hname,"RITZ_PARIS");
        else if (strcmp(i.destination, "LONDON") == 0) strcpy(i.hname,"RITZ_LONDON");
        else if (strcmp(i.destination, "SYDNEY") == 0) strcpy(i.hname,"PARK_HYATT_SYDNEY");
        else if (strcmp(i.destination, "NEW_YORK") == 0) strcpy(i.hname,"FOUR_SEASONS_NY");
        else if (strcmp(i.destination, "TOKYO") == 0) strcpy(i.hname,"PARK_HYATT_TOKYO");
        else if (strcmp(i.destination, "PAKISTAN") == 0) strcpy(i.hname,"MARRIOTT_ISB");
        else strcpy(i.hname,"INTERNATIONAL_GRAND");
    } else if (n == 2) {
        i.cost += i.days * base2;
        if (strcmp(i.destination, "ITALY") == 0) strcpy(i.hname,"NH_COLLECTION");
        else if (strcmp(i.destination, "FRANCE") == 0) strcpy(i.hname,"FOUR_SEASONS_PARIS");
        else if (strcmp(i.destination, "LONDON") == 0) strcpy(i.hname,"SHANGRI_LA_LONDON");
        else if (strcmp(i.destination, "SYDNEY") == 0) strcpy(i.hname,"SHANGRI_LA_SYDNEY");
        else if (strcmp(i.destination, "NEW_YORK") == 0) strcpy(i.hname,"MANDARIN_ORIENTAL_NY");
        else if (strcmp(i.destination, "TOKYO") == 0) strcpy(i.hname,"AMAN_TOKYO");
        else if (strcmp(i.destination, "PAKISTAN") == 0) strcpy(i.hname,"AURA_LAHORE");
        else strcpy(i.hname,"CITY_HOTEL");
    } else {
        i.cost += i.days * base3;
        if (strcmp(i.destination, "ITALY") == 0) strcpy(i.hname,"HOTEL_DE_RUSSIE");
        else if (strcmp(i.destination, "FRANCE") == 0) strcpy(i.hname,"SOFITEL_PARIS");
        else if (strcmp(i.destination, "LONDON") == 0) strcpy(i.hname,"HILTON_PARK");
        else if (strcmp(i.destination, "SYDNEY") == 0) strcpy(i.hname,"NOVOTEL_SYDNEY");
        else if (strcmp(i.destination, "NEW_YORK") == 0) strcpy(i.hname,"HYATT_CENTRAL_NY");
        else if (strcmp(i.destination, "TOKYO") == 0) strcpy(i.hname,"HOTEL_GRAND_PACIFIC");
        else if (strcmp(i.destination, "PAKISTAN") == 0) strcpy(i.hname,"PEARL_CONTINENTAL");
        else strcpy(i.hname,"ECONO_INN");
    }

    printf("ENTER DATE OF CHECKIN (day month year): ");
    if (scanf("%d %d %d", &d, &m, &y) != 3) { while(getchar()!='\n'); d=1;m=1;y=2025; }
    while(getchar()!='\n');

    /* compute checkout by adding days using correct month lengths */
    d1 = d; m1 = m; yr1 = y;
    int remaining = i.days;
    while (remaining > 0) {
        int dim = days_in_month(m1, yr1);
        if (d1 + remaining <= dim) {
            d1 += remaining;
            remaining = 0;
        } else {
            remaining -= (dim - d1 + 1);
            d1 = 1;
            m1++;
            if (m1 > 12) { m1 = 1; yr1++; }
        }
    }

    printf("\nPress enter to continue ...");
    getchar();
}

/* flights: add flight cost and set flight fields based on destination */
void flights(int id) {
    int choice;
    /* show options based on destination */
    if (strcmp(i.destination, "ITALY") == 0) {
        printf("\n FLIGHTS TO Italy:\n1. ITA_AIRWAYS 60000 07.00 AM\n2. TURKISH_AIRLINES 65000 10.00 AM\n3. EMIRATES 70000 11.00 PM\n");
    } else if (strcmp(i.destination, "FRANCE") == 0) {
        printf("\n FLIGHTS TO France:\n1. AIR_FRANCE 62000 06.30 AM\n2. TURKISH_AIRLINES 64000 09.00 AM\n3. EMIRATES 72000 10.00 PM\n");
    } else if (strcmp(i.destination, "LONDON") == 0) {
        printf("\n FLIGHTS TO London:\n1. BRITISH_AIRWAYS 63000 06.00 AM\n2. TURKISH_AIRLINES 60000 08.30 AM\n3. EMIRATES 75000 07.00 PM\n");
    } else if (strcmp(i.destination, "SYDNEY") == 0) {
        printf("\n FLIGHTS TO Sydney:\n1. QANTAS 80000 08.00 AM\n2. EMIRATES 85000 09.30 PM\n3. SINGAPORE_AIRLINES 78000 11.00 AM\n");
    } else if (strcmp(i.destination, "NEW_YORK") == 0) {
        printf("\n FLIGHTS TO New York:\n1. EMIRATES 90000 09.00 PM\n2. BRITISH_AIRWAYS 88000 07.00 AM\n3. QATAR_AIRWAYS 86000 10.30 PM\n");
    } else if (strcmp(i.destination, "TOKYO") == 0) {
        printf("\n FLIGHTS TO Tokyo:\n1. JAL 70000 06.00 AM\n2. ANA 72000 09.00 AM\n3. QATAR_AIRWAYS 75000 11.00 PM\n");
    } else if (strcmp(i.destination, "PAKISTAN") == 0) {
        printf("\n FLIGHTS (Domestic/Regional):\n1. PIA 15000 07.00 AM\n2. AIR_SIAL 12000 09.00 AM\n3. SERENE_AIR 14000 03.00 PM\n");
    } else {
        printf("\n FLIGHTS (General):\n1. MAJOR_CARRIER_1 60000 07.00 AM\n2. MAJOR_CARRIER_2 55000 09.00 AM\n3. MAJOR_CARRIER_3 58000 05.00 PM\n");
    }

    printf("ENTER CHOICE (1-3): ");
    if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); choice = 1; }
    while(getchar()!='\n');

    /* assign cost, time and airline name based on destination and choice */
    if (strcmp(i.destination, "ITALY") == 0) {
        if (choice == 1) { i.cost += 60000; i.time = 7.00; strcpy(i.shift,"AM"); strcpy(i.fname,"ITA_AIRWAYS"); }
        else if (choice == 2) { i.cost += 65000; i.time = 10.00; strcpy(i.shift,"AM"); strcpy(i.fname,"TURKISH_AIRLINES"); }
        else { i.cost += 70000; i.time = 23.00; strcpy(i.shift,"PM"); strcpy(i.fname,"EMIRATES"); }
    } else if (strcmp(i.destination, "FRANCE") == 0) {
        if (choice == 1) { i.cost += 62000; i.time = 6.30; strcpy(i.shift,"AM"); strcpy(i.fname,"AIR_FRANCE"); }
        else if (choice == 2) { i.cost += 64000; i.time = 9.00; strcpy(i.shift,"AM"); strcpy(i.fname,"TURKISH_AIRLINES"); }
        else { i.cost += 72000; i.time = 22.00; strcpy(i.shift,"PM"); strcpy(i.fname,"EMIRATES"); }
    } else if (strcmp(i.destination, "LONDON") == 0) {
        if (choice == 1) { i.cost += 63000; i.time = 6.00; strcpy(i.shift,"AM"); strcpy(i.fname,"BRITISH_AIRWAYS"); }
        else if (choice == 2) { i.cost += 60000; i.time = 8.30; strcpy(i.shift,"AM"); strcpy(i.fname,"TURKISH_AIRLINES"); }
        else { i.cost += 75000; i.time = 19.00; strcpy(i.shift,"PM"); strcpy(i.fname,"EMIRATES"); }
    } else if (strcmp(i.destination, "SYDNEY") == 0) {
        if (choice == 1) { i.cost += 80000; i.time = 8.00; strcpy(i.shift,"AM"); strcpy(i.fname,"QANTAS"); }
        else if (choice == 2) { i.cost += 85000; i.time = 21.30; strcpy(i.shift,"PM"); strcpy(i.fname,"EMIRATES"); }
        else { i.cost += 78000; i.time = 11.00; strcpy(i.shift,"AM"); strcpy(i.fname,"SINGAPORE_AIRLINES"); }
    } else if (strcmp(i.destination, "NEW_YORK") == 0) {
        if (choice == 1) { i.cost += 90000; i.time = 21.00; strcpy(i.shift,"PM"); strcpy(i.fname,"EMIRATES"); }
        else if (choice == 2) { i.cost += 88000; i.time = 7.00; strcpy(i.shift,"AM"); strcpy(i.fname,"BRITISH_AIRWAYS"); }
        else { i.cost += 86000; i.time = 22.30; strcpy(i.shift,"PM"); strcpy(i.fname,"QATAR_AIRWAYS"); }
    } else if (strcmp(i.destination, "TOKYO") == 0) {
        if (choice == 1) { i.cost += 70000; i.time = 6.00; strcpy(i.shift,"AM"); strcpy(i.fname,"JAL"); }
        else if (choice == 2) { i.cost += 72000; i.time = 9.00; strcpy(i.shift,"AM"); strcpy(i.fname,"ANA"); }
        else { i.cost += 75000; i.time = 23.00; strcpy(i.shift,"PM"); strcpy(i.fname,"QATAR_AIRWAYS"); }
    } else if (strcmp(i.destination, "PAKISTAN") == 0) {
        if (choice == 1) { i.cost += 15000; i.time = 7.00; strcpy(i.shift,"AM"); strcpy(i.fname,"PIA"); }
        else if (choice == 2) { i.cost += 12000; i.time = 9.00; strcpy(i.shift,"AM"); strcpy(i.fname,"AIR_SIAL"); }
        else { i.cost += 14000; i.time = 15.00; strcpy(i.shift,"PM"); strcpy(i.fname,"SERENE_AIR"); }
    } else {
        if (choice == 1) { i.cost += 60000; i.time = 7.00; strcpy(i.shift,"AM"); strcpy(i.fname,"MAJOR_CARRIER_1"); }
        else if (choice == 2) { i.cost += 55000; i.time = 9.00; strcpy(i.shift,"AM"); strcpy(i.fname,"MAJOR_CARRIER_2"); }
        else { i.cost += 58000; i.time = 17.00; strcpy(i.shift,"PM"); strcpy(i.fname,"MAJOR_CARRIER_3"); }
    }
}

/* destination selection */
void destination() {
    system("cls");
    int n;
    printf("\n DESTINATION :\n1. ITALY\n2. FRANCE\n3. LONDON\n4. SYDNEY\n5. NEW YORK\n6. TOKYO\n7. PAKISTAN TOUR\n");
    printf("ENTER CHOICE : ");
    if (scanf("%d", &n) != 1) { while(getchar()!='\n'); return; }
    while(getchar()!='\n');

    int cnt;
    switch (n) {
        case 1: printf("WELCOME TO ITALY. ENTER NO. OF PEOPLE: "); if (scanf("%d",&cnt)==1){ while(getchar()!='\n'); input_batch(cnt,1);} break;
        case 2: printf("WELCOME TO FRANCE. ENTER NO. OF PEOPLE: "); if (scanf("%d",&cnt)==1){ while(getchar()!='\n'); input_batch(cnt,2);} break;
        case 3: printf("WELCOME TO LONDON. ENTER NO. OF PEOPLE: "); if (scanf("%d",&cnt)==1){ while(getchar()!='\n'); input_batch(cnt,3);} break;
        case 4: printf("WELCOME TO SYDNEY. ENTER NO. OF PEOPLE: "); if (scanf("%d",&cnt)==1){ while(getchar()!='\n'); input_batch(cnt,4);} break;
        case 5: printf("WELCOME TO NEW YORK. ENTER NO. OF PEOPLE: "); if (scanf("%d",&cnt)==1){ while(getchar()!='\n'); input_batch(cnt,5);} break;
        case 6: printf("WELCOME TO TOKYO. ENTER NO. OF PEOPLE: "); if (scanf("%d",&cnt)==1){ while(getchar()!='\n'); input_batch(cnt,6);} break;
        case 7: printf("WELCOME TO PAKISTAN TOUR. ENTER NO. OF PEOPLE: "); if (scanf("%d",&cnt)==1){ while(getchar()!='\n'); input_batch(cnt,7);} break;
        default: printf("Invalid choice.\n"); break;
    }
}

/* iterative menu (no recursion) */
int menu() {
    int n;
    while (1) {
        system("cls");
        printf("\nMENU :\n1. CHOOSE DESTINATION\n2. GENERATE BILL (search by ID)\n3. EXIT\nENTER CHOICE : ");
        if (scanf("%d", &n) != 1) { while(getchar()!='\n'); n = 0; }
        while(getchar()!='\n');

        switch (n) {
            case 1: destination(); break;
            case 2: print_details(); break;
            case 3: return 0;
            default: printf("Invalid choice. Press enter to continue..."); getchar(); break;
        }
    }
    return 0;
}

int main() {
    system("cls");
    printf("\n\n\tWELCOME\n\tHUZAIFA AND HASAN TRAVEL AGENCY\n\nPress enter to continue ...");
    getchar();
    menu();
    return 0;
}