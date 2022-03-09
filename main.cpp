/*Links de referencia:
http://libreallegro.blogspot.com
https://htmlcolorcodes.com/es/
https://es.slideshare.net/residentejf/manual-allegro-c
https://es.scribd.com/doc/6765291/Tutorial-Allegro
https://www.allegro.cc/manual/4/
*/

// Proyecto personal:Juego de Aterrizaje lunar //

#include <iostream>
#include <allegro.h>
#include <cmath>
#include <cstdlib>

using namespace std;

//Funciones para el vehiculo
void aceleracion(float da,float &vx,float &vy);
void Rotacion(float &x , float &y , float Cx , float Cy , float da);
void Construir_Nave(float Cx,float Cy,BITMAP *buffer);//Funcion para construir la nave
void Gravedad_enNave(float &Cx,float &Cy,float &Vx,float &Vy); //Funcion para simular gravedad
void Propulsion(float da,float Cx,float Cy,BITMAP *buffer);
void medidor_combustible(bool Rconsumo ,float &Rcombustible, bool consumo,float &combustible,BITMAP *buffer);

//Funciones para el escenario
void Niveles(int level,BITMAP *buffer);
void Explosion(float Cx,float Cy,BITMAP *buffer,int level);
bool Fin_del_juego(float Vy,float Cx,float Cy,BITMAP *buffer,int level);
bool Aterrizaje(float Cx,float Cy,float Vx,float Vy,BITMAP *buffer,int level);
bool Colisiones(float x1,float y1,float x2,float y2,float p1x,float p1y,float p2x,float p2y);
bool Colision_S(int level,float Cx,float Cy);
void siguiente_Nivel(float &Cx,float &Cy,float &Vx,float &Vy,int &level,float &combustible,float &Rcombustible,BITMAP *buffer);
bool MalAterrizaje(float Cx,float Cy,float Vx,float Vy,BITMAP *buffer,int level);

//Funciones para el menu
int MenuDeInicio(int &level,BITMAP *buffer);

int main(){
    cout << "Iniciando programa" << endl;

    allegro_init(); //Funcion para inicializar o instalar las funciones de allegro. el entorno grafico(Allegro).No tiene parametros

    install_keyboard(); //Funcion para recibir la entrada del teclado

    set_color_depth(32); //Numero de Bits que ocupara la ventana(Formato de pixeles)

    set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,500,0,0); //Funcion para definir el tamaño de la pantalla


    BITMAP *buffer = create_bitmap(800,500);

    float Cx,Cy;
    float Vx,Vy;

    float combustible = 100;
    float Rcombustible = 100;


    int level;
        level = 0;
            do{
            MenuDeInicio(level,buffer);
            }while(level == 0 && !key[KEY_ESC]);
            if(level == 1){
              Cx = 120; Cy = 432;
            }if(level == 2){
             Cx = 100 ; Cy = 100;
            }


    while(!key[KEY_ESC] && !Fin_del_juego(Vy,Cx,Cy,buffer,level) /*&& !Aterrizaje(Cx,Cy,Vx,Vy,buffer,level)*/){//Ciclo While para cerrar el programa cuando se apreta la tecla ESC
        bool consumo = false ;
        bool Rconsumo = false ;


        clear_to_color(buffer, 0x000000);






        Niveles(level,buffer);

        Gravedad_enNave(Cx,Cy,Vx,Vy);


        //Revisar Warnings                                              //
        textprintf(buffer,font,300,15,0xEAECEE,"Velocidad %.2f",(Vy));

        if(key[KEY_UP] && combustible > 0){
            aceleracion(0,Vx,Vy);
            Propulsion(0,Cx,Cy,buffer);
            consumo = true ;
        }
        if(key[KEY_RIGHT] && Rcombustible > 0){
            aceleracion(-90,Vx,Vy);
            Propulsion(-90,Cx,Cy,buffer);
            Rconsumo = true;
        }
        if(key[KEY_LEFT] && Rcombustible > 0){
            aceleracion(90,Vx,Vy);
            Propulsion(90,Cx,Cy,buffer);
            Rconsumo = true;
        }


        medidor_combustible(Rconsumo,Rcombustible,consumo,combustible,buffer);
        Construir_Nave(Cx,Cy,buffer);
        Aterrizaje(Cx,Cy,Vx,Vy,buffer,level);
        if(Aterrizaje(Cx,Cy,Vx,Vy,buffer,level) == true){
            siguiente_Nivel(Cx,Cy,Vx,Vy,level,combustible,Rcombustible,buffer);
        }


        if(Fin_del_juego(Vy,Cx,Cy,buffer,level) == true || MalAterrizaje(Cx,Cy,Vx,Vy,buffer,level) == true){ //Retorno al punto de partida
            if(level == 1){
            level = 1;
            Cx = 120; Cy = 432;
            Vy = 0; Vx = 0;
            combustible = 100; Rcombustible = 100;
            Niveles(level,buffer);
            }if(level == 2){
             level = 2;
             Cx = 100 ; Cy = 100;
             Vy = 0; Vx = 0;
             combustible = 100; Rcombustible = 100;
             Niveles(level,buffer);
            }

        }
        if(key[KEY_R]){ //Reinicio
            level = 1;
            Cx = 120; Cy = 432;
            Vy = 0; Vx = 0;
            combustible = 100; Rcombustible = 100;
            Niveles(level,buffer);

        }






        blit(buffer,screen,0,0,0,0,800,500);
        rest(20); //Funcion para parar la ejecucion del programa como el delay()

    }
    cout << "Fin del programa" << endl;
    return 0;

}
END_OF_MAIN(); //Especificador para determinar el final de ejecucion del programa

int MenuDeInicio(int &level,BITMAP *buffer){
    int opcion;
    int CUx ;
    int CUy ;



clear_bitmap(buffer);
//Titulo
textout_centre_ex(buffer,font,"Operacion Aterrizaje",138,135,0xEAECEE,0x000000);
line(buffer,58,145,215,145,0xEAECEE);
//Opciones
textout_centre_ex(buffer,font,"Seleccion de niveles:",142,165,0xEAECEE,0x000000);
textout_centre_ex(buffer,font,"Nivel 1",142,180,0xEAECEE,0x000000);
textout_centre_ex(buffer,font,"Nivel 2",142,194,0xEAECEE,0x000000);
//Cursor de control
triangle(buffer,CUx,CUy,CUx+5,CUy-2,CUx+5,CUy+2,0xEAECEE);

        if(key[KEY_UP]){
        opcion--;
        rest(60);
    }if(key[KEY_DOWN]){
        opcion++;
        rest(60);
    }
    if(key[KEY_SPACE] && opcion == 1){
        return level=1;
    }
    if(key[KEY_SPACE] && opcion == 2){
        return level=2;
    }


    if(opcion < 1 || opcion > 2){
        opcion = 1;
    }

    switch (opcion){
    case 1:{
        opcion =1;
       CUx = 240;
       CUy = 184;

    break;
    }
    case 2:{
        opcion =2;
       CUx = 240;
       CUy = 198;
    break;
    }

    /*default:
        opcion = 1;*/
    }
blit(buffer,screen,0,0,0,0,800,500);
rest(50);








    }


void siguiente_Nivel(float &Cx,float &Cy,float &Vx,float &Vy,int &level,float &combustible,float &Rcombustible,BITMAP *buffer){

        if(Aterrizaje(Cx,Cy,Vx,Vy,buffer,level) == true){

            level += 1;
            if(level == 2){
              Cx = 100 ; Cy = 100;
              Vx = 0 ; Vy = 0;
              combustible = 100; Rcombustible = 100;
            }
            if(level > 2){
                Cx = 100 ; Cy = 100;
            Vx = 0 ; Vy = 0;
                level = 2;
              combustible = 100; Rcombustible = 100;
            }
        }
}

bool Colision_S(int level,float Cx,float Cy){//Justifica si hubo una colision en la superficie con el vehiculo
        int Tx1[9]={0,100,200,300,400,500,600,700,800};
        float r1x = Cx-10 , r1y = Cy-15 ;
        float r2x = Cx+10 , r2y = Cy+5 ;
        if(level == 1 ){
            if(Colisiones(0,450,200,445,r1x,r1y,r2x,r2y) == true){
                return true;
            }if(Colisiones(200,445,400,450,r1x,r1y,r2x,r2y) == true){
                return true;
            }if(Colisiones(400,450,600,445,r1x,r1y,r2x,r2y) == true){
                return true;
            }if(Colisiones(600,445,800,450,r1x,r1y,r2x,r2y) == true){
                return true;
            }
        }if(level == 2){
        int Ty1[9]={450,445,430,435,450,445,445,425,445};
        int Ty2[9]={445,430,435,450,445,445,425,445,450};

        for(int i=0;i<=7;i++ ){
        if(Colisiones(Tx1[i],Ty1[i],Tx1[i+1],Ty2[i],r1x,r1y,r2x,r2y) == true){
                return true;
            }
         }

        }

        return false;
}

bool Colisiones(float x1,float y1,float x2,float y2,float p1x,float p1y,float p2x,float p2y){
        float m = (y2-y1)/(x2-x1);
        // m>0 colision derecha, m<0 colision izquierda
        float b = y1-m*x1;
        if(m>0){
            if(x1 <= p1x && p1x <= x2){
                if(p2y >= m*p1x + b){
                    return true;
                    }
            }else{
                if(p1x <= x1 && x1<= p2x){
                    if(y1 <= p2y){
                    return true;
                    }
                }
            }
        }else if(m<0){
            if(x1 <= p2x && p2x <= x2){
                if(p2y >= m*p2x + b){
                    return true;
                    }
            }else{
                if(p1x <= x2 && x2<= p2x){
                    if(y2 <= p2y){
                    return true;
                    }
                }
            }
        }
    return false;
}

bool MalAterrizaje(float Cx,float Cy,float Vx,float Vy,BITMAP *buffer,int level){
        if(Colision_S(level,Cx,Cy) == true && Vy <=1.5){

                do{
            //Texto
            textout_centre_ex(buffer,font,"Aterrizaje No valido",400,200,0xEAECEE,0x000000);
            Vy = 0;
            blit(buffer,screen,0,0,0,0,800,500);
            rest(10);
        if(key[KEY_ESC]){
                return true;
            }

        }while(!key[KEY_SPACE]);

                return true;
        }
}

bool Aterrizaje(float Cx,float Cy,float Vx,float Vy,BITMAP *buffer,int level){
        //Deteccion de plataforma de Inicio
        if(level == 1){
        if(Cy+6 >= 440 && Cx-10 >= 100 && Cx+10 <= 140){
          if(Vy <=0.5){
          do{
            //Texto
            textout_centre_ex(buffer,font,"Oprima Arriba para despegar",400,200,0xEAECEE,0x000000);
            Cy = 437;
            Vy = 0;
            blit(buffer,screen,0,0,0,0,800,500);
            rest(10);

            if(key[KEY_ESC]){
                return true;
            }
        }while(!key[KEY_UP]);
          }else{
          Explosion(Cx,Cy,buffer,level);
          }
        }
        }


        //Deteccion de aterrizaje en plataforma de aterrizaje
        if(level == 1){
        if(Cy+6 >= 442 && Cx-10 >= 580 && Cx+10 <= 620){
            if(Vy <=1.5){
                do{
            //Texto
            textout_centre_ex(buffer,font,"Aterrizaje Completado",400,200,0xEAECEE,0x000000);
            Vy = 0;
            blit(buffer,screen,0,0,0,0,800,500);
            rest(10);
            if(key[KEY_ESC]){
                return true;
            }
        }while(!key[KEY_SPACE]);
                return true;
            }else{
                Explosion(Cx,Cy,buffer,level);
            }
        }
    }
        if(level == 2){
        if(Cy+6 >= 443 && Cx-10 >= 520 && Cx+10 <= 560){
            if(Vy <=1.5){
                do{
            //Texto
            textout_centre_ex(buffer,font,"Aterrizaje Completado",400,200,0xEAECEE,0x000000);
            Vy = 0;
            blit(buffer,screen,0,0,0,0,800,500);
            rest(10);
            if(key[KEY_ESC]){
                return true;
            }
        }while(!key[KEY_SPACE]);
                return true;
            }else{
                Explosion(Cx,Cy,buffer,level);
            }
        }
        }
        return false;
}

bool Fin_del_juego(float Vy,float Cx,float Cy,BITMAP *buffer,int level){
        if(Cx+10 >= 800 || Cx-10 <= 0 || Cy-15 <=0 || Cy+5 >= 500){
              Explosion(Cx,Cy,buffer,level);

                return true;


        }
        if(Colision_S(level,Cx,Cy) == true && Vy >=1.5){
            Explosion(Cx,Cy,buffer,level);

                return true;


        }


        return false;
}

void Explosion(float Cx,float Cy,BITMAP *buffer,int level){
        float x[12] = {Cx-10,Cx-10,Cx-5,Cx+5,Cx+10,Cx+10,Cx-10,Cx-5,Cx-5,Cx+5,Cx-5,Cx+5};
        float y[12] = {Cy+5,Cy-5, Cy ,Cy ,Cy+5,Cy-5,Cy-10,Cy-15,Cy-10,Cy-10,Cy-15,Cy-15};


        float dx[6] = {3,3,0,-3,-3,0};
        float dy[6] = {0,-3,-3,-3,0,3};
        clear(screen);
        do{
            int j=0;
            clear(buffer);
            Niveles(level,buffer);
            for(int i=0;i<=10;i+=2){
                line(buffer,x[i],y[i],x[i+1],y[i+1],0xEAECEE);
                Rotacion(x[i+1],y[i+1],x[i],y[i],5);
                x[i] += dx[j];
                y[i] += dy[j];
                x[i+1] += dx[j];
                y[i+1] += dy[j];
                j++;
            }
            //Texto
            textout_centre_ex(buffer,font,"Aterrizaje fallido",400,200,0xEAECEE,0x000000);

            blit(buffer,screen,0,0,0,0,800,500);
            rest(10);


        }while(!key[KEY_ESC] && !key[KEY_SPACE]);

        }

void Niveles(int level,BITMAP *buffer){
        int rand (void);
        int Tx1[9]={0,100,200,300,400,500,600,700,800};
        /*int Ty1 = 440 + rand() % (450-440);
        int Ty2 = 440 + rand() % (450-440);
        */
        int Ty1[9]={450,445,430,435,450,445,445,425,445};
        int Ty2[9]={445,430,435,450,445,445,425,445,450};

        switch (level){
        case 1 :{
            //Terreno definido
            line(buffer,0,450,200,445,0xEAECEE);
            line(buffer,200,445,400,450,0xEAECEE);
            line(buffer,400,450,600,445,0xEAECEE);
            line(buffer,600,445,800,450,0xEAECEE);
            //Plataforma de aterrizaje
            rectfill(buffer,580,440,620,442,0xEAECEE);//Base
            rect(buffer,580,440,582,445,0xEAECEE);
            rect(buffer,618,440,620,445,0xEAECEE);
            //Plataforma de Inicio
            rectfill(buffer,100,440,140,442,0xEAECEE);
            rect(buffer,100,440,102,448,0xEAECEE);
            rect(buffer,140,440,138,446,0xEAECEE);
            break;
        }
        case 2 ... 10 :{
            //Plataforma de aterrizaje
            rectfill(buffer,520,445,560,443,0xEAECEE);//Base
            //Generacion de Terreno Aleatorio , Prototipo
            for(int i=0;i<=7;i++ ){
            line(buffer,Tx1[i],Ty1[i],Tx1[i+1],Ty2[i],0xEAECEE);
            }
            //Falta desarrollar

            break;
        }
    }





        }




void medidor_combustible(bool Rconsumo ,float &Rcombustible,bool consumo,float &combustible,BITMAP *buffer){
        textout_centre_ex(buffer,font,"Fuel ",45,15,0xEAECEE,0x000000);
        rectfill(buffer,30,30,30+combustible,32,0xEAECEE);
        if(consumo == true){
            combustible -= 0.1;
        }
        textout_centre_ex(buffer,font,"RCS Fuel ",200,15,0xEAECEE,0x000000);
        rectfill(buffer,170,30,170+Rcombustible,32,0xEAECEE);
        if(Rconsumo == true){
            Rcombustible -= 0.1;
        }
        //Revisar Warnings                                              //
        textprintf(buffer,font,80,15,0xEAECEE,"%.2f",(combustible));
        textprintf(buffer,font,240,15,0xEAECEE,"%.2f",(Rcombustible));
}

void Propulsion(float da,float Cx,float Cy,BITMAP *buffer){
        float c1,c2;
        c1 = Cx; c2=Cy;



        if(da == 90){
        c1+=6;
        c2+=10;
        float RetroProp[6] = {c1-2,c2,c1,c2+5,c1+2,c2};
        for(int i=0;i <5 ;i +=2 ){
            Rotacion(RetroProp[i],RetroProp[i+1],Cx,Cy,da);
        }
        for(int i=0;i <4 ;i +=2 ){
            line(buffer,RetroProp[i],RetroProp[i+1],RetroProp[i+2],RetroProp[i+3],0xEAECEE);
        }
       }

        if(da == -90){
        c1-=6;
        c2+=10;
        float RetroProp[6] = {c1-2,c2,c1,c2+5,c1+2,c2};
        for(int i=0;i <5 ;i +=2 ){
            Rotacion(RetroProp[i],RetroProp[i+1],Cx,Cy,da);
        }
        for(int i=0;i <4 ;i +=2 ){
            line(buffer,RetroProp[i],RetroProp[i+1],RetroProp[i+2],RetroProp[i+3],0xEAECEE);
        }
       }

        float Llama[6] = {c1-5,c2,c1,c2+5,c1+5,c2};
        if(da == 0){
        for(int i=0;i <4 ;i +=2 ){
            line(buffer,Llama[i],Llama[i+1],Llama[i+2],Llama[i+3],0xEAECEE);
        }
        }

}

// Fisicas de la nave //

void aceleracion(float da,float &Vx,float &Vy){
        float Ax=0, Ay= -0.15;
        Rotacion(Ax,Ay,0,0,da);
        Vx += Ax;
        Vy += Ay;

}

void Gravedad_enNave(float &Cx,float &Cy,float &Vx,float &Vy){
        float Ax,Ay;
        Ax = 0;     //Aceleracion
        Ay = 0.05;

        Vx += Ax;   //Velocidad
        Vy += Ay;

        Cx += Vx;
        Cy += Vy;

}


void Rotacion(float &x , float &y , float Cx , float Cy , float da){
        float dx = x - Cx;
        float dy = y - Cy;
        float r = sqrt(dx*dx + dy*dy);
        float a = atan2(dy,dx);

        float da_rad = da/180 * M_PI;
        a -= da_rad;
        x = Cx + r*cos(a);
        y = Cy + r*sin(a);

}

// Diseño de Nave //

void Construir_Nave(float Cx,float Cy,BITMAP *buffer){
        line(buffer,Cx-10,Cy+5 ,Cx-10,Cy,0xEAECEE); //Pata Izquierda
        line(buffer,Cx-10,Cy+5 ,Cx-5,Cy,0xEAECEE);
        //line(buffer,Cx-10,Cy ,Cx-5,Cy,0xEAECEE);

        line(buffer,Cx+10,Cy+5 ,Cx+10,Cy,0xEAECEE); //Pata Derecha
        line(buffer,Cx+10,Cy+5 ,Cx+5,Cy,0xEAECEE);
        //line(buffer,Cx+10,Cy ,Cx+5,Cy,0xEAECEE);

        //line(buffer,Cx-5,Cy ,Cx+5,Cy,0xEAECEE);        //Base o motor
        //line(buffer,Cx-10,Cy-5 ,Cx+10,Cy-5,0xEAECEE);

        rect(buffer,Cx-10,Cy-10 ,Cx+10,Cy,0xEAECEE); //Cuerpo y cabeza
        rect(buffer,Cx-5,Cy-15 ,Cx+5,Cy-10,0xEAECEE);

        line(buffer,Cx-10,Cy-10 ,Cx-5,Cy-15,0xEAECEE);
        line(buffer,Cx-10,Cy-10 ,Cx-5,Cy-10,0xEAECEE);
        line(buffer,Cx-5,Cy-15 ,Cx-5,Cy-10,0xEAECEE);

        line(buffer,Cx+5,Cy-15 ,Cx+10,Cy-10,0xEAECEE);
        line(buffer,Cx+5,Cy-15 ,Cx+5,Cy-10,0xEAECEE);
        line(buffer,Cx+10,Cy-10 ,Cx+5,Cy-10,0xEAECEE);

}
