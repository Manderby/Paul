
typedef struct Param Param;

Param* allocParam(void);
void deallocParam(Param* con);

double getParamValue(const Param* param);
void setParamValue(Param* param, double value);

double getParamMin(const Param* param);
void setParamMin(Param* param, double min);

double getParamMax(const Param* param);
void setParamMax(Param* param, double max);
