

/*
int spatDim, specDim;
specDim = PTB_GetSpecDim();
spatDim = 2; //PTB_GetSpatDim();
ATB_SetAcqDimDesc(specDim, spatDim, NULL );
*/
//ACQ_dim = 2;

//PVM_SpatDimEnum =  2D;//D;//DIM_ENUM_TYPE;


printf("\nPVM_Matrix:");
for (i = 0; i < (sizeof PVM_Matrix) / (sizeof PVM_Matrix[0]); i++ ) {
  printf("%d,", PVM_Matrix[i]);
}
printf("\nPVM_EncMatrix:");
for (i = 0; i < (sizeof PVM_EncMatrix) / (sizeof PVM_EncMatrix[0]); i++ ) {
  printf("%d,", PVM_Matrix[i]);
}
backbone();
printf("\nPVM_Matrix:");
for (i = 0; i < (sizeof PVM_Matrix) / (sizeof PVM_Matrix[0]); i++ ) {
  printf("%d,", PVM_Matrix[i]);
}
printf("\nPVM_EncMatrix:");
for (i = 0; i < (sizeof PVM_EncMatrix) / (sizeof PVM_EncMatrix[0]); i++ ) {
  printf("%d,", PVM_Matrix[i]);
}
