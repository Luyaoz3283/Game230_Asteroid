#include "globalVariable.h"




Vector2f globalVariable::screenDim(1200, 1000);


Vector2f globalVariable::getUnitVector(Vector2f vector)
{
	float magnititude = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
	return vector / magnititude;
}
