#include	<stdio.h>
#include	<stdlib.h>
#include    <string.h>
#include    <stdbool.h>

typedef struct RegisteredVehicle{ //for one registered vehicle
char* license_plate; //to be used for string license plate# of the vehicle
char* owner_name; //to be used for storing owner name
} RegisteredVehicle;

typedef struct Garage{ //for one garage
char* garage_name; //to be used for garage name
int total_capacity; //total capacity of the garage
int current_count; // number of vehicles in the garage in a particular time
RegisteredVehicle** parked_vehicles; //list of parked vehicles in a particular time
} Garage;

typedef struct Campus{
Garage** garages; //list of garages in campus
int total_garages; //number of garages in the campus
RegisteredVehicle** registered_vehicles; //list of registered vehicles
int total_registered_vehicles; //total number of registered vehicles
} Campus;

int removeVehicleFromGarage(Garage* garage, const char* license);

int maxL=21;

Garage* createGarage(const char* name, int capacity);

RegisteredVehicle* createRegisteredVehicle(const char* license, const char* owner);

void parkVehicle(Garage* garage, RegisteredVehicle* vehicle);

RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license);

Garage *findGarage(const Campus* campus, const char* garage);

void generateGarageUtilizationReport(const Campus* campus);

int resizeGarage(Garage* garage, int new_capacity);

int removeVehicleFromGarage(Garage* garage, const char* license);

int relocateVehicle(Campus* campus, const char* license, const char* target_garage_name);

int countTotalVehicles(const Campus* campus);

void registerVehicle(Campus* campus, const char* license, const char* owner);

int removeGarage(Campus* campus, const char* garage_name);

void freeCampus(Campus* campus);

void freeGarage(Garage *garage);


int main(){

    int g; // # of garages
    int v; // # of registered vehicles
    int c; // # of commands to be processed
    scanf("%d %d %d", &g, &v, &c);

    Campus campus;
    campus.garages = (Garage **)malloc(g*sizeof(Garage *)); //creates garage list double pointer with space * number of garage pointers!
    campus.total_garages = g;
    campus.registered_vehicles = (RegisteredVehicle** )malloc(v*sizeof(RegisteredVehicle *)); //creates list of registered vehicles
    campus.total_registered_vehicles = v;




    // SCANNING GARAGES //

    for(int i=0; i<g; i++){

        char name[maxL];

        int capacity;

        scanf("%s %d",name, &capacity);

        campus.garages[i] = createGarage(name,capacity);

    }


    // SCANNING DRIVERS //

    for(int i=0; i<v; i++){

        char name[maxL];

        char license[maxL];
        
        scanf("%s %s",license,name);

        campus.registered_vehicles[i] = createRegisteredVehicle(license, name);

    }

    // COMMANDS //

    for(int i=0; i<c; i++){

        char command[maxL+1];

        scanf("%s", command);

        getchar();

        // PARK //

        // PARK COMMAND //
        if(strcmp(command, "PARK")==0){

            //getchar();

            char license[maxL+1];
            char garage[maxL+1];

            scanf("%s %s", license, garage);

            //getchar();

            //processPark(&campus,licenses[parkCount],garages[parkCount]);

            RegisteredVehicle *car = searchVehicleByLicense(&campus, license); //finds car

            Garage *targetGarage; 

            //Garage *targetGarage = findGarage(&campus, garage); //finds garage, is going to first garage :(
            
            for(int j=0; j<campus.total_garages; j++){

                if(strcmp(campus.garages[j]->garage_name,garage)==0){

                    //campus.garages[j]->current_count++;
                    targetGarage = campus.garages[j];
                    //campus.garages[j]->current_count++;
                    //printf("garage found");
                    break;

            } else {
                //printf("garage not found");
                continue;
            }
        }

            parkVehicle(targetGarage, car); //parks it!!
        }

        //  UTILIZATION REPORT COMMAND //
        if(strcmp(command, "UTILIZATION_REPORT")==0){

            generateGarageUtilizationReport(&campus);

        }

        // RESIZE COMMAND //
        if(strcmp(command, "RESIZE")==0){

            int r;
            int newC;
            char garage[maxL+1];

            Garage *tGarage; 

            scanf("%s %d", garage, &newC);

            //printf("%s",garage);
            
            for(int i=0; i<campus.total_garages; i++){

                if(strcmp(campus.garages[i]->garage_name,garage)==0){

                    //printf("%s",garage);

                    tGarage = campus.garages[i];
                    r = resizeGarage(tGarage,newC);
                    if(r==1){
                    printf("SUCCESS\n");
                    break;

                    } 
                } else {
                    continue;
                } 
            }
        }

        // SEARCH_OWNER COMMAND //
        if(strcmp(command,"SEARCH_OWNER")==0){

            //SEARCH_OWNER ANEESHA//
            char name[maxL+1];

            //char license[maxL+1];

            //char garage[maxL+1];

            //RegisteredVehicle *n;


            scanf("%s",name);

            //printf("%s\n",name); gets the names

            int found = 0;


            //check if person is registered
            for(int j=0; j<campus.total_registered_vehicles; j++){

                //printf("%s\n ", campus.registered_vehicles[j]->owner_name); it prints all of them it knows it gets out of the loop too early

                if(strcmp(campus.registered_vehicles[j]->owner_name,name)==0){

                     //printf("%s" , campus.registered_vehicles[j]->owner_name);

                    found = 1;
                    int parked = 0;

                    for(int k=0; k<campus.total_garages; k++){

                        for(int z=0; z<campus.garages[k]->current_count; z++){

                            if(strcmp(campus.garages[k]->parked_vehicles[z]->license_plate, campus.registered_vehicles[j]->license_plate)==0){

                                printf("%s %s\n", campus.registered_vehicles[j]->license_plate, campus.garages[k]->garage_name);
                                parked = 1;
                                break;

                            }
                        }
                        if(parked) break;
                    }
                    if(!parked){
                            printf("%s NOT ON CAMPUS\n", campus.registered_vehicles[j]->license_plate );
                        }
                } 
            }
            if(!found){
                printf("NO REGISTERED CAR BY THIS OWNER\n"); 
            }
            }

        // REMOVE VEHICLE COMMAND //
        if(strcmp(command,"REMOVE_VEHICLE_GARAGE")==0){

            char license[maxL+1];

            scanf("%s", license);

            int removed =0;

            //lets check where and if its on campus
            for(int j=0; j<campus.total_garages; j++){
                for(int z=0; z<campus.garages[j]->current_count; z++){
                    if(strcmp(campus.garages[j]->parked_vehicles[z]->license_plate, license)==0){

                        // if on campus, we call the remove from garage function
                        removed = removeVehicleFromGarage(campus.garages[j], campus.garages[j]->parked_vehicles[z]->license_plate);
                        if(removed){
                            printf("REMOVED FROM %s\n", campus.garages[j]->garage_name);
                        }
                        break;
                        //found = 1;
                    
                    }

                }
            } if (!removed){
                printf("NOT FOUND IN CAMPUS\n");
            }

        }

        // RELOCATE COMMAND //
        if(strcmp(command,"RELOCATE")==0){

            char license[maxL +1];

            char garage[maxL +1];

            scanf("%s %s", license, garage);

            int relocate = 0;

            relocate =  relocateVehicle(&campus, license, garage);

            if(relocate){
                printf("RELOCATION SUCCESSFUL\n");
            }
        }
        
        //COUNT_TOTAL COMMAND// 
        if(strcmp(command,"COUNT_TOTAL")==0){

            int total = 0;

            total = countTotalVehicles(&campus);
        
            printf("%d\n",total);

        }
        
        // REGISTER COMMAND //
        if(strcmp(command,"REGISTER_VEHICLE")==0){

            char name[maxL+1];
            char license[maxL+1];

            scanf("%s %s", license, name);

            registerVehicle(&campus,license,name);

        }
        
        // REMOVE GARAGE COMMAND //
        if(strcmp(command,"REMOVE_GARAGE")==0){

            char garage[maxL+1];
            int p =0;
            scanf("%s",garage);

            for(int i=0; i<campus.total_garages; i++){
                if(strcmp(campus.garages[i]->garage_name,garage)==0){

                    p = removeGarage(&campus, garage);
                    printf("%s REMOVED\n", garage);
                
                }

            }  
            if(!p){
                printf("%s NOT FOUND\n",garage);
            }

        }
        }

     // FREE //

     freeCampus(&campus);
}

void freeGarage(Garage *garage) {

    free(garage->garage_name);
    free(garage->parked_vehicles);
    free(garage);

}

void freeCampus(Campus* campus){
    for(int i=0; i<campus->total_garages; i++){

        freeGarage(campus->garages[i]);

    }
    free(campus->garages);

    for(int i=0; i<campus->total_registered_vehicles; i++){
        free(campus->registered_vehicles[i]->license_plate);
        free(campus->registered_vehicles[i]->owner_name);
        free(campus->registered_vehicles);
    }
}



Garage* createGarage(const char* name, int capacity){

    Garage *g = (Garage *)malloc(sizeof(Garage));

    g->parked_vehicles = (RegisteredVehicle **)malloc(sizeof(RegisteredVehicle *) * capacity);

    g->garage_name = (char *)malloc(sizeof(char)*(strlen(name)+1));

    g->total_capacity = capacity; 

    strcpy(g->garage_name,name);

    g->current_count = 0;

    return g;

}

RegisteredVehicle* createRegisteredVehicle(const char* license, const char* owner){

    RegisteredVehicle* p = (RegisteredVehicle *)malloc(sizeof(RegisteredVehicle));

    p->license_plate = (char *)malloc(sizeof(char)*(strlen(license)+1));

    strcpy(p->license_plate,license);

    p->owner_name = (char *)malloc(sizeof(char)*(strlen(owner)+1));

    strcpy(p->owner_name,owner);

    return p;
}

void parkVehicle(Garage* garage, RegisteredVehicle* vehicle){
    
    if(garage->current_count < garage->total_capacity){

        garage->parked_vehicles[garage->current_count] = vehicle;

        garage->current_count++;

        printf("PARKED\n");

    } else 
        printf("FULL\n");
} 

RegisteredVehicle* searchVehicleByLicense(const Campus* campus, const char* license){


    RegisteredVehicle* result = NULL;

        for (int j=0; j < campus->total_registered_vehicles; j++) {

                if(strcmp(campus->registered_vehicles[j]->license_plate,license)==0){

                    result = campus->registered_vehicles[j];

                    break; //exits inner loop

                } 

                //if (found) break; //if found is true loop breaks
            }
    return result;
}

Garage *findGarage(const Campus* campus, const char* g){

    Garage *result = NULL;

    //thisis the issue to find park, when i turn true it gives segmentation fault 
    for(int i=0; i<campus->total_garages; i++){

        if(strcmp(campus->garages[i]->garage_name,g)==0){

            result = campus->garages[i];
            break;
            printf("found %s",campus->garages[i]->garage_name);

        }else 
        printf("garage not found");
    }

    return result; 

}

void generateGarageUtilizationReport(const Campus* campus){

    Garage *garage = campus->garages[0];

    float min= 100.00;

    for(int i=0; i<campus->total_garages; i++){

        float utilization = ((float)campus->garages[i]->current_count / campus->garages[i]->total_capacity) * 100.00; 

        printf("Garage: %s, Capacity: %d, Occupied: %d, Utilization: %.02f%% \n",campus->garages[i]->garage_name,campus->garages[i]->total_capacity,campus->garages[i]->current_count, utilization);

        if(utilization < min){

            min = utilization;
            garage = campus->garages[i];

            }
    }

    printf("Least Utilized: %s\n", garage->garage_name);
}

int resizeGarage(Garage* garage, int new_capacity){

    //printf("%d",garage->current_count);
    if(garage->current_count > new_capacity){

        printf("FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n");
        return 0;

    }else{

    garage->total_capacity = new_capacity;

    return 1;

    }
}

//Removes a vehicle from a specified garage. The function returns 1 if the removal is successful. Otherwise, it returns 0.
int removeVehicleFromGarage(Garage* garage, const char* license){

    for(int i=0; i<garage->current_count; i++){

        if(strcmp(garage->parked_vehicles[i]->license_plate, license)==0){

            garage->current_count--;
            return 1;
            break;

        }
    } 

    return 0;
}

int relocateVehicle(Campus* campus, const char* license, const char* target_garage_name){

    int foundtg =0;
    RegisteredVehicle *car;
    Garage *garage1;
    Garage *garage2;

    //printf("%s %s\n", license, target_garage_name);

//find target garage

    for(int j=0; j<campus->total_garages; j++){

        //check if garage exists 
        if(strcmp(campus->garages[j]->garage_name,target_garage_name)==0){

            //printf("%s\n",campus->garages[j]->garage_name); //target garage

            garage1 = campus->garages[j];

            //target garage found 
            foundtg = 1;

        }

        }if(!foundtg){
            printf("%s NOT FOUND\n", target_garage_name);
            }

    //find if person is in campus

    int foundg =0;

    for(int i=0; i<campus->total_garages; i++){

        for(int j=0; j<campus->garages[i]->current_count; j++){

            //printf("%s %s\n",campus->garages[i]->parked_vehicles[j]->license_plate,campus->garages[i]->garage_name);
            //check if person is in campus

            if(strcmp(campus->garages[i]->parked_vehicles[j]->license_plate, license)==0){

            car = campus->garages[i]->parked_vehicles[j];

            garage2 = campus->garages[i];
            
            //printf("%s\n",campus->garages[i]->garage_name); //current garage

            foundg=1;

            break;

            //printf("found %s parked in %s", campus->garages[i]->parked_vehicles[j]->license_plate,campus->garages[j]->garage_name);

        } 
    } 

} if(!foundg){
        printf("%s NOT ON CAMPUS\n", license);
    }
// if person is both in campus and the garage is real
if(foundg && foundtg){
    if(garage1->current_count < garage1->total_capacity){
        parkVehicle(garage1,car);
        removeVehicleFromGarage(garage2, license);
        return 1;
}else
printf("%s IS FULL\n", garage1->garage_name);
}


return 0;
    
}

int countTotalVehicles(const Campus* campus){
    int total = 0;

            for(int i=0; i<campus->total_garages; i++){

                total+= campus->garages[i]->current_count;

            }

    return total;

}

void registerVehicle(Campus* campus, const char* license, const char* owner){

    //int m = campus->total_registered_vehicles;

    //RegisteredVehicle* newVehicle = realloc(campus->registered_vehicles,(campus->total_registered_vehicles+1)*sizeof(RegisteredVehicle));
    RegisteredVehicle *car = createRegisteredVehicle(license,owner);

    campus->registered_vehicles = (RegisteredVehicle**)realloc(campus->registered_vehicles, sizeof(RegisteredVehicle*) * (campus->total_registered_vehicles + 1));
    
    campus->registered_vehicles[campus->total_registered_vehicles] = car;

    //strcpy(campus->registered_vehicles[campus->total_registered_vehicles]->license_plate, license);
    //strcpy(campus->registered_vehicles[campus->total_registered_vehicles]->owner_name,owner);

    campus->total_registered_vehicles++;

    printf("REGISTERED\n");

}

int removeGarage(Campus* campus, const char* garage_name){

    int p = 0;

    for(int i=0; i<campus->total_garages; i++){
        if(strcmp(campus->garages[i]->garage_name,garage_name)==0){
        for(int j=0; j<campus->garages[i]->current_count; j++){    

            free(campus->garages[i]->parked_vehicles[j]);

        }
            free(campus->garages[i]->garage_name);
            free(campus->garages[i]);
            p = 1;
            

            //campus->total_garages--;
            for(int j=i; j<campus->total_garages; j++){

                campus->garages[j] = campus->garages[j+1];

            }

            campus->total_garages--;
        }
    }
    return p;
}

/*
Resizes a garage to a new capacity. If the
new capacity is smaller than the current number of vehicles in that garage, then you should print a
massage "FAIL. TOO SMALL TO ACCOMMODATE EXISTING VEHICLES.\n". The function
returns 1 or 0 depending on whether the task was successful or not.

3 15 7
GARAGE_A 3
GARAGE_B 4
GARAGE_C 3
ABC123 AHMED
XYZ789 MIRAZIZ
DEF456 AISHA
XYZ787 HUDSON
XYZ689 WILLIAM
XYZ779 MUHAMMAD
XYZ589 KEVIN
DEF455 ARJUN
DEF756 MATTHEW
DGF456 RICARDO
ABC1335 JAMES
ABD123 ANEESHA
ABD723 ANEESHA
ACC123 RYAN
ADC1234 ZENA
PARK DEF756 GARAGE_C
PARK ABD723 GARAGE_C
PARK ABC1335 GARAGE_A
PARK XYZ589 GARAGE_C
UTILIZATION_REPORT
PARK DEF455 GARAGE_C
RESIZE GARAGE_A 5


void registerVehicle(Campus* campus, const char* license, const char* owner){

    RegisteredVehicle *car = createRegisteredVehicle(license,owner);

    //increase MDA with realloc
    campus->registered_vehicles = (RegisteredVehicle**)realloc(campus->registered_vehicles, sizeof(RegisteredVehicle*) * (campus->total_registered_vehicles + 1));
    
    campus->registered_vehicles[campus->total_registered_vehicles] = car;
    
    campus->total_registered_vehicles++;

}
*/