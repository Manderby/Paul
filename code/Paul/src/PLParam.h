


typedef struct PLParam PLParam;



// Alloc and dealloc parameters
PLParam* plAllocParam(void);
void plDeallocParam(PLParam* param);

// Getter and Setter
const NAUTF8Char* plGetParamName(const PLParam* param);
void plSetParamName(PLParam* param, const NAUTF8Char* name);

double plGetParamValue(const PLParam* param);
void plSetParamValue(PLParam* param, double value);

double plGetParamMin(const PLParam* param);
void plSetParamMin(PLParam* param, double min);

double plGetParamMax(const PLParam* param);
void plSetParamMax(PLParam* param, double max);
