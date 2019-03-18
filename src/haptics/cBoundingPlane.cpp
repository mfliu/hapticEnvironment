#include "cBoundingPlane.h"

cBoundingPlane::cBoundingPlane(cVector3d* topLeft, cVector3d* topRight, cVector3d* botLeft, cVector3d* botRight, int stiffness)
{
  topEdge = new cMesh();
  topEdge->newVertex(*topLeft);
  topEdge->newVertex(*topRight);
  topEdge->m_material->setStiffness(stiffness);
  
  bottomEdge = new cMesh();
  bottomEdge->newVertex(*botLeft);
  bottomEdge->newVertex(*botRight); 
  bottomEdge->m_material->setStiffness(stiffness);

  leftEdge = new cMesh();
  leftEdge->newVertex(*topLeft);
  leftEdge->newVertex(*botLeft);
  leftEdge->m_material->setStiffness(stiffness);

  rightEdge = new cMesh();
  rightEdge->newVertex(*topRight);
  rightEdge->newVertex(*botRight);
  rightEdge->m_material->setStiffness(stiffness);
}
