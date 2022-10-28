// Prof. André Gustavo Schaeffer
// UFFS - Campus Erechim(RS)
// Este programa foi criado para fins educativos e NÃO está protegido por nenhum direito autoral.
// Seja livre!

#define pinoRed          6  
#define pinoGreen        9
#define pinoBlue        10
#define pinoLDR         A1
#define TempoLeituras   70

int IteracoesDeAprendizado, Erro, Aux, MenorNumErros=9999, VTemp[8], m[18][8], i, j, ValorLDR;
int CorEmProcesso, Erros, Estado; 
// Variável "Estado" =1 quando em geracao de historico, =2 quando em ajuste de pesos (aprendizado), e =3 quando em validação do modelo 

double MaiorSaida, n, v0, v1, v2, v3, v4, v5, v6, Saida;
double GuardaPeso0, GuardaPeso1, GuardaPeso2, GuardaPeso3, GuardaPeso4, GuardaPeso5, GuardaPeso6;
double peso0, peso1, peso2, peso3, peso4, peso5, peso6;

char Cor;

void setup()
{ 
  Serial.begin(9600);
  randomSeed(analogRead(A0));   Serial.begin(9600);   pinMode(pinoRed,   OUTPUT);   pinMode(pinoGreen, OUTPUT);   pinMode(pinoBlue,  OUTPUT);   pinMode(pinoLDR,   INPUT);
  digitalWrite(pinoRed,   HIGH);   digitalWrite(pinoGreen, HIGH);   digitalWrite(pinoBlue,  HIGH);
  
  //Estado =1 quando em geracao de historico, =2 quando em ajuste de pesos (aprendizado), e =3 quando em validação do modelo 
  Estado=3;
  
  //CORES: 1 AZUL   2 VERMELHO   3 VERDE   
  CorEmProcesso=3;
  IteracoesDeAprendizado=1000;
  n=0.1;    
    
// 1 AZUL   
m[0][0]=25.00; m[0][1]=41.00; m[0][2]=94.00; m[0][3]=18.00; m[0][4]=20.00; m[0][5]=34.00; m[0][6]=17.00; m[0][7]=1;
m[1][0]=24.00; m[1][1]=41.00; m[1][2]=92.00; m[1][3]=19.00; m[1][4]=20.00; m[1][5]=33.00; m[1][6]=16.00; m[1][7]=1;
m[2][0]=24.00; m[2][1]=40.00; m[2][2]=90.00; m[2][3]=18.00; m[2][4]=20.00; m[2][5]=32.00; m[2][6]=16.00; m[2][7]=1;
m[3][0]=24.00; m[3][1]=40.00; m[3][2]=91.00; m[3][3]=18.00; m[3][4]=20.00; m[3][5]=33.00; m[3][6]=16.00; m[3][7]=1;
m[4][0]=23.00; m[4][1]=39.00; m[4][2]=85.00; m[4][3]=17.00; m[4][4]=19.00; m[4][5]=31.00; m[4][6]=15.00; m[4][7]=1;
m[5][0]=24.00; m[5][1]=40.00; m[5][2]=90.00; m[5][3]=18.00; m[5][4]=20.00; m[5][5]=33.00; m[5][6]=16.00; m[5][7]=1;

  
// 2 VERMELHO   
m[6][0]=16.00; m[6][1]=41.00; m[6][2]=95.00; m[6][3]=14.00; m[6][4]=15.00; m[6][5]=34.00; m[6][6]=13.00; m[6][7]=2;
m[7][0]=16.00; m[7][1]=40.00; m[7][2]=91.00; m[7][3]=13.00; m[7][4]=14.00; m[7][5]=33.00; m[7][6]=12.00; m[7][7]=2;
m[8][0]=16.00; m[8][1]=40.00; m[8][2]=93.00; m[8][3]=13.00; m[8][4]=14.00; m[8][5]=34.00; m[8][6]=12.00; m[8][7]=2;
m[9][0]=16.00; m[9][1]=41.00; m[9][2]=93.00; m[9][3]=13.00; m[9][4]=14.00; m[9][5]=34.00; m[9][6]=12.00; m[9][7]=2;
m[10][0]=19.00; m[10][1]=41.00; m[10][2]=95.00; m[10][3]=15.00; m[10][4]=16.00; m[10][5]=34.00; m[10][6]=14.00; m[10][7]=2;
m[11][0]=16.00; m[11][1]=41.00; m[11][2]=91.00; m[11][3]=13.00; m[11][4]=14.00; m[11][5]=34.00; m[11][6]=12.00; m[11][7]=2;


// 3 VERDE   
m[12][0]=24.00; m[12][1]=37.00; m[12][2]=95.00; m[12][3]=18.00; m[12][4]=20.00; m[12][5]=31.00; m[12][6]=16.00; m[12][7]=3;
m[13][0]=22.00; m[13][1]=30.00; m[13][2]=83.00; m[13][3]=15.00; m[13][4]=18.00; m[13][5]=26.00; m[13][6]=14.00; m[13][7]=3;
m[14][0]=24.00; m[14][1]=34.00; m[14][2]=93.00; m[14][3]=17.00; m[14][4]=20.00; m[14][5]=29.00; m[14][6]=16.00; m[14][7]=3;
m[15][0]=24.00; m[15][1]=32.00; m[15][2]=89.00; m[15][3]=16.00; m[15][4]=19.00; m[15][5]=28.00; m[15][6]=15.00; m[15][7]=3;
m[16][0]=24.00; m[16][1]=33.00; m[16][2]=93.00; m[16][3]=17.00; m[16][4]=20.00; m[16][5]=29.00; m[16][6]=15.00; m[16][7]=3;
m[17][0]=24.00; m[17][1]=35.00; m[17][2]=93.00; m[17][3]=17.00; m[17][4]=20.00; m[17][5]=30.00; m[17][6]=16.00; m[17][7]=3;



  //Randomiza entradas da matriz

  for (i=0; i<(18/2); i++)
  { 
   if(i % 2 != 0)
   { 
     VTemp[0]=m[i][0]; VTemp[1]=m[i][1]; VTemp[2]=m[i][2]; VTemp[3]=m[i][3]; VTemp[4]=m[i][4]; VTemp[5]=m[i][5]; VTemp[6]=m[i][6]; VTemp[7]=m[i][7];
     m[i][0]=m[i+(18/2)][0]; m[i][1]=m[i+(18/2)][1]; m[i][2]=m[i+(18/2)][2]; m[i][3]=m[i+(18/2)][3];  m[i][4]=m[i+(18/2)][4]; m[i][5]=m[i+(18/2)][5]; m[i][6]=m[i+(18/2)][6]; m[i][7]=m[i+(18/2)][7];
     m[i+(18/2)][0]=VTemp[0];  m[i+(18/2)][1]=VTemp[1]; m[i+(18/2)][2]=VTemp[2]; m[i+(18/2)][3]=VTemp[3];  m[i+(18/2)][4]=VTemp[4]; m[i+(18/2)][5]=VTemp[5]; m[i+(18/2)][6]=VTemp[6]; m[i+(18/2)][7]=VTemp[7];
   }
  }

  Serial.println("Matriz randomizada:"); 
  for (i=0; i<18; i++) { 
    Serial.print(m[i][0]);Serial.print(" ");  Serial.print(m[i][1]);Serial.print(" ");  Serial.print(m[i][2]);Serial.print(" ");  Serial.print(m[i][3]); Serial.print(" ");
    Serial.print(m[i][4]);Serial.print(" ");  Serial.print(m[i][5]);Serial.print(" ");  Serial.print(m[i][6]);Serial.print(" ");  Serial.println(m[i][7]); 
    }


  if(Estado==1)
  {
  
  Serial.println("Pressione S e ENTER para iniciar a leitura da cor - geracao de dados de historico:");
  while(!Serial.available());
  delay(1000);

  // 
  for(j=0; j<6; j++)
  {
   digitalWrite(pinoRed,   LOW);                                                                delay(TempoLeituras); v0 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoRed,   HIGH); delay(TempoLeituras);
   digitalWrite(pinoGreen, LOW);                                                                delay(TempoLeituras); v1 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoGreen, HIGH); delay(TempoLeituras);
   digitalWrite(pinoBlue,  LOW);                                                                delay(TempoLeituras); v2 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoBlue,  HIGH); delay(TempoLeituras);
   digitalWrite(pinoRed,   LOW); digitalWrite(pinoGreen,   LOW);                                delay(TempoLeituras); v3 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoRed,   HIGH); digitalWrite(pinoGreen,   HIGH);                                  delay(TempoLeituras);
   digitalWrite(pinoRed,   LOW); digitalWrite(pinoBlue,    LOW);                                delay(TempoLeituras); v4 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoRed,   HIGH); digitalWrite(pinoBlue,    HIGH);                                  delay(TempoLeituras);
   digitalWrite(pinoGreen, LOW); digitalWrite(pinoBlue,    LOW);                                delay(TempoLeituras); v5 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoGreen, HIGH); digitalWrite(pinoBlue,    HIGH);                                  delay(TempoLeituras);
   digitalWrite(pinoRed,   LOW); digitalWrite(pinoGreen,   LOW); digitalWrite(pinoBlue,   LOW); delay(TempoLeituras); v6 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoRed,   HIGH); digitalWrite(pinoGreen,   HIGH);  digitalWrite(pinoBlue,   HIGH); delay(TempoLeituras);
  
   Serial.print("m["); Serial.print(j+(6*(CorEmProcesso-1))); Serial.print("][0]="); Serial.print(v0);  Serial.print  ("; ");  
   Serial.print("m["); Serial.print(j+(6*(CorEmProcesso-1))); Serial.print("][1]="); Serial.print(v1);  Serial.print  ("; "); 
   Serial.print("m["); Serial.print(j+(6*(CorEmProcesso-1))); Serial.print("][2]="); Serial.print(v2);  Serial.print  ("; "); 
   Serial.print("m["); Serial.print(j+(6*(CorEmProcesso-1))); Serial.print("][3]="); Serial.print(v3);  Serial.print  ("; "); 
   Serial.print("m["); Serial.print(j+(6*(CorEmProcesso-1))); Serial.print("][4]="); Serial.print(v4);  Serial.print  ("; "); 
   Serial.print("m["); Serial.print(j+(6*(CorEmProcesso-1))); Serial.print("][5]="); Serial.print(v5);  Serial.print  ("; "); 
   Serial.print("m["); Serial.print(j+(6*(CorEmProcesso-1))); Serial.print("][6]="); Serial.print(v6);  Serial.print  ("; "); 
   Serial.print("m["); Serial.print(j+(6*(CorEmProcesso-1))); Serial.print("][7]="); Serial.print(CorEmProcesso); Serial.println(";"); 

   delay(4000);
  }
 }
}

void loop()
{

  if(Estado ==2)
  {
   Serial.println("Pressione S e ENTER para iniciar o aprendizado:");
   while(!Serial.available());

    for (j=0; j<IteracoesDeAprendizado; j++)
    {
     Erros=0;
     for (i=0; i<18; i++)
     {
         v0 = m[i][0]   ; 
         v1 = m[i][1]   ;
         v2 = m[i][2]   ; 
         v3 = m[i][3]   ; 
         v4 = m[i][4]   ; 
         v5 = m[i][5]   ; 
         v6 = m[i][6]   ; 
                                                     
         if((j==0) && (i==0))
         { 
           peso0=random(9); peso1=random(9); peso2=random(9); peso3=random(9); peso4=random(9); peso5=random(9); peso6=random(9);            
           Aux=random(2); if(Aux==1)peso0=peso0*-1;   Aux=random(2); if(Aux==1)peso1=peso1*-1;   Aux=random(2); if(Aux==1)peso2=peso2*-1;   Aux=random(2); if(Aux==1)peso3=peso3*-1;   Aux=random(2); if(Aux==1)peso4=peso4*-1;   Aux=random(2); if(Aux==1)peso5=peso5*-1;   Aux=random(2); if(Aux==1)peso6=peso6*-1;          
          }

         Saida = peso0*v0 + peso1*v1 + peso2*v2 + peso3*v3 + peso4*v4 + peso5*v5 + peso6*v6;
                  
         Erro=0;
         if((Saida >=0)&&(m[i][7]==CorEmProcesso))        ;
         if((Saida >=0)&&(m[i][7]!=CorEmProcesso)) Erro=-1;
         if((Saida < 0)&&(m[i][7]==CorEmProcesso)) Erro= 1;
         if((Saida < 0)&&(m[i][7]!=CorEmProcesso))        ;

         if(Erro!=0)
         {
           Erros = Erros +1;
           peso0 = peso0 + n*Erro*v0;     peso1 = peso1 + n*Erro*v1;     peso2 = peso2 + n*Erro*v2;   peso3 = peso3 + n*Erro*v3;   peso4 = peso4 + n*Erro*v4;   peso5 = peso5 + n*Erro*v5;   peso6 = peso6 + n*Erro*v6;   
          }
    
    }
  
     if(MenorNumErros > Erros) 
     {
       MenorNumErros = Erros;
       GuardaPeso0 = peso0;     GuardaPeso1 = peso1;     GuardaPeso2 = peso2;     GuardaPeso3 = peso3;     GuardaPeso4 = peso4;     GuardaPeso5 = peso5;     GuardaPeso6 = peso6; 
       Serial.print("Erros: "); Serial.println(Erros);
     }

    }  // fim do FOR de leitura da Matriz

    Serial.print("Menor numero de Erros: "); Serial.println(MenorNumErros);
    peso0 = GuardaPeso0; peso1 = GuardaPeso1; peso2 = GuardaPeso2; peso3 = GuardaPeso3; peso4 = GuardaPeso4; peso5 = GuardaPeso5; peso6 = GuardaPeso6; 
 
    Serial.println("Melhores pesos para a cor sendo analisada:"); 
    Serial.print(" peso0 = "); Serial.print(peso0); Serial.print("; ");  Serial.print("peso1 = "); Serial.print(peso1); Serial.print("; ");  Serial.print("peso2 = "); Serial.print(peso2); Serial.print("; ");  Serial.print("peso3 = "); Serial.print(peso3); Serial.print("; "); Serial.print("peso4 = "); Serial.print(peso4); Serial.print("; "); Serial.print("peso5 = "); Serial.print(peso5); Serial.print("; "); Serial.print("peso6 = "); Serial.print(peso6); Serial.print("; "); 

    delay(2000); // para garantir que havera tempo de escrever os dados no monitor serial antes do "exit"
    exit(0); // aborta o programa porque por ora só se buscavam os pesos

  }  // Fim do bloco de aprendizado 
  

   if(Estado==3)
   {
    j=1;   
    while(true)
    {
     if(analogRead(pinoLDR) > 350) // quando mais alto maior eh a "quantidade de escuridao" que ele precisa para iniciar as leituras
     {
      delay(1500);
      
      digitalWrite(pinoRed,   LOW);                                                                delay(TempoLeituras); v0 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoRed,   HIGH); delay(TempoLeituras);
      digitalWrite(pinoGreen, LOW);                                                                delay(TempoLeituras); v1 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoGreen, HIGH); delay(TempoLeituras);
      digitalWrite(pinoBlue,  LOW);                                                                delay(TempoLeituras); v2 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoBlue,  HIGH); delay(TempoLeituras);
      digitalWrite(pinoRed,   LOW); digitalWrite(pinoGreen,   LOW);                                delay(TempoLeituras); v3 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoRed,   HIGH); digitalWrite(pinoGreen,   HIGH);                                  delay(TempoLeituras);
      digitalWrite(pinoRed,   LOW); digitalWrite(pinoBlue,    LOW);                                delay(TempoLeituras); v4 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoRed,   HIGH); digitalWrite(pinoBlue,    HIGH);                                  delay(TempoLeituras);
      digitalWrite(pinoGreen, LOW); digitalWrite(pinoBlue,    LOW);                                delay(TempoLeituras); v5 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoGreen, HIGH); digitalWrite(pinoBlue,    HIGH);                                  delay(TempoLeituras);
      digitalWrite(pinoRed,   LOW); digitalWrite(pinoGreen,   LOW); digitalWrite(pinoBlue,   LOW); delay(TempoLeituras); v6 = analogRead(pinoLDR); delay(TempoLeituras); digitalWrite(pinoRed,   HIGH); digitalWrite(pinoGreen,   HIGH);  digitalWrite(pinoBlue,   HIGH); delay(TempoLeituras);
     
      Serial.print(j); Serial.print(":");
      MaiorSaida=-9999999;
      Cor = ' ';
 
     // AZUL
     peso0 = 0.90; peso1 = 15.00; peso2 = -10.70; peso3 = 2.30; peso4 = 8.80; peso5 = 0.50; peso6 = 11.50;
     Saida = peso0 * v0 + peso1 * v1 + peso2 * v2 + peso3 * v3 + peso4 * v4 + peso5 * v5 + peso6 * v6;
     if(Saida >= 0) if (Saida > MaiorSaida) { MaiorSaida = Saida; Cor='B'; } 
      
     // VERMELHO
     peso0 = -38.70; peso1 = 21.80; peso2 = 9.20; peso3 = -23.20; peso4 = -33.90; peso5 = 19.40; peso6 = -19.60;
     Saida = peso0 * v0 + peso1 * v1 + peso2 * v2 + peso3 * v3 + peso4 * v4 + peso5 * v5 + peso6 * v6;
     if(Saida >= 0) if (Saida > MaiorSaida) { MaiorSaida = Saida; Cor='R'; } 
     
     // VERDE
     peso0 = 15.50; peso1 = -34.70; peso2 = 9.80; peso3 = 2.80; peso4 = 21.10; peso5 = -18.00; peso6 = 10.50; 
     Saida = peso0 * v0 + peso1 * v1 + peso2 * v2 + peso3 * v3 + peso4 * v4 + peso5 * v5 + peso6 * v6;
     if(Saida >= 0) if (Saida > MaiorSaida) { MaiorSaida = Saida; Cor='G'; } 

     if(Cor == 'B') { Serial.print("Azul! ");     } 
     if(Cor == 'R') { Serial.print("Vermelho! "); } 
     if(Cor == 'G') { Serial.print("Verde! ");    } 

     delay(3000);
     j++;
     Serial.println();
    }
   }  // Fim do While(true)
  }
}
