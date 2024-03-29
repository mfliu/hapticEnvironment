#include "cBoundingPlane.h"

/**
 * @param stiffness Stiffness of the bounding planes 
 * @param toolRadius Size of the haptic cursor in the workspace
 * @param bWidth Width of area in the bounding plane 
 * @param bHeight Height of area in the bounding plane
 *
 * Initializes meshes for each side of the bounding plane and ensures that the haptic tool can enter
 * the bounding box but can't leave it, by setting the directionality of the vertex normals.
 */

cBoundingPlane::cBoundingPlane(int stiffness, double toolRadius, double bWidth, double bHeight)
{
  double w = bWidth/2;
  double h = bHeight/2;
  cVector3d p0l = cVector3d(-0.05, w, h);
  cVector3d p1l = cVector3d(-0.05, -w, h);
  cVector3d p2l = cVector3d(-0.05, w, -h);
  cVector3d p3l = cVector3d(-0.05, -w, -h);
  
  cVector3d p0u = cVector3d(0.05, w, h);
  cVector3d p1u = cVector3d(0.05, -w, h);
  cVector3d p2u = cVector3d(0.05, w, -h);
  cVector3d p3u = cVector3d(0.05, -w, -h);

  lowerMesh = new cMesh();
  lowerMesh->setLocalPos(0.0, 0.0, 0.0);
  lowerMesh->setUseCulling(false);
  int vertex0l = lowerMesh->newVertex();
  int vertex1l = lowerMesh->newVertex();
  int vertex2l = lowerMesh->newVertex();
  int vertex3l = lowerMesh->newVertex();
  lowerMesh->m_vertices->setLocalPos(vertex0l, p0l);
  lowerMesh->m_vertices->setLocalPos(vertex1l, p1l);
  lowerMesh->m_vertices->setLocalPos(vertex2l, p2l);
  lowerMesh->m_vertices->setLocalPos(vertex3l, p3l);
  lowerMesh->newTriangle(vertex0l, vertex1l, vertex2l);
  lowerMesh->newTriangle(vertex2l, vertex1l, vertex3l);
  lowerMesh->computeAllNormals();
  lowerMesh->computeBoundaryBox(true);
  lowerMesh->createAABBCollisionDetector(toolRadius);
  lowerMesh->setStiffness(0.08*stiffness);
  lowerMesh->setShowTriangles(false);
  lowerMesh->setUseMaterial(true);
  lowerMesh->m_material->setUseHapticFriction(true);
  lowerMesh->m_material->setDynamicFriction(0.0);
  lowerMesh->m_material->setStaticFriction(0.0);
  lowerMesh->m_material->setAudioFrictionGain(0.0);
  lowerMesh->m_material->setAudioImpactGain(0.0);
  lowerMesh->m_material->setVibrationFrequency(0.0);
  lowerMesh->m_material->setDamping(0.0);
  lowerMesh->m_material->setHapticTriangleSides(true, false);
  
  upperMesh = new cMesh();
  upperMesh->setLocalPos(0.0, 0.0, 0.0);
  upperMesh->setUseCulling(false);
  int vertex0u = upperMesh->newVertex();
  int vertex1u = upperMesh->newVertex();
  int vertex2u = upperMesh->newVertex();
  int vertex3u = upperMesh->newVertex();
  upperMesh->m_vertices->setLocalPos(vertex0u, p0u);
  upperMesh->m_vertices->setLocalPos(vertex1u, p1u);
  upperMesh->m_vertices->setLocalPos(vertex2u, p2u);
  upperMesh->m_vertices->setLocalPos(vertex3u, p3u);
  upperMesh->m_vertices->setNormal(vertex0u, cVector3d(-1.0, 0.0, 0.0));
  upperMesh->m_vertices->setNormal(vertex1u, cVector3d(-1.0, 0.0, 0.0));
  upperMesh->m_vertices->setNormal(vertex2u, cVector3d(-1.0, 0.0, 0.0));
  upperMesh->newTriangle(vertex0u, vertex1u, vertex2u);
  upperMesh->newTriangle(vertex2u, vertex1u, vertex3u);
  upperMesh->computeAllNormals();
  upperMesh->computeBoundaryBox(true);
  upperMesh->createAABBCollisionDetector(toolRadius);
  upperMesh->setStiffness(0.08*stiffness);
  upperMesh->setShowTriangles(false);
  upperMesh->setUseMaterial(true);
  upperMesh->m_material->setUseHapticFriction(true);
  upperMesh->m_material->setDynamicFriction(0.0);
  upperMesh->m_material->setStaticFriction(0.0);
  upperMesh->m_material->setAudioFrictionGain(0.0);
  upperMesh->m_material->setAudioImpactGain(0.0);
  upperMesh->m_material->setDamping(0.0);
  upperMesh->m_material->setVibrationFrequency(0.0);
  upperMesh->m_material->setHapticTriangleSides(false, true);

  topMesh = new cMesh();
  topMesh->setLocalPos(0.0, 0.0, 0.0);
  topMesh->setUseCulling(false);
  int vertex0t = topMesh->newVertex();
  int vertex1t = topMesh->newVertex();
  int vertex2t = topMesh->newVertex();
  int vertex3t = topMesh->newVertex();
  topMesh->m_vertices->setLocalPos(vertex0t, p0l);
  topMesh->m_vertices->setLocalPos(vertex1t, p1l);
  topMesh->m_vertices->setLocalPos(vertex2t, p0u);
  topMesh->m_vertices->setLocalPos(vertex3t, p1u);
  topMesh->newTriangle(vertex0t, vertex1t, vertex2t);
  topMesh->newTriangle(vertex2t, vertex1t, vertex3t);
  topMesh->computeAllNormals();
  topMesh->computeBoundaryBox(true);
  topMesh->createAABBCollisionDetector(toolRadius);
  topMesh->setStiffness(0.08*stiffness);
  topMesh->setShowTriangles(false);
  topMesh->m_material->setHapticTriangleSides(false, true);

  bottomMesh = new cMesh();
  bottomMesh->setLocalPos(0.0, 0.0, 0.0);
  bottomMesh->setUseCulling(false);
  int vertex0b = bottomMesh->newVertex();
  int vertex1b = bottomMesh->newVertex();
  int vertex2b = bottomMesh->newVertex();
  int vertex3b = bottomMesh->newVertex();
  bottomMesh->m_vertices->setLocalPos(vertex0b, p2l);
  bottomMesh->m_vertices->setLocalPos(vertex1b, p3l);
  bottomMesh->m_vertices->setLocalPos(vertex2b, p2u);
  bottomMesh->m_vertices->setLocalPos(vertex3b, p3u);
  bottomMesh->newTriangle(vertex0b, vertex1b, vertex2b);
  bottomMesh->newTriangle(vertex2b, vertex1b, vertex3b);
  bottomMesh->computeAllNormals();
  bottomMesh->computeBoundaryBox(true);
  bottomMesh->createAABBCollisionDetector(toolRadius);
  bottomMesh->setStiffness(0.08*stiffness);
  bottomMesh->setShowTriangles(false);
  bottomMesh->m_material->setHapticTriangleSides(true, false);
  
  leftMesh = new cMesh();
  leftMesh->setLocalPos(0.0, 0.0, 0.0);
  leftMesh->setUseCulling(false);
  int vertex0le = leftMesh->newVertex();
  int vertex1le = leftMesh->newVertex();
  int vertex2le = leftMesh->newVertex();
  int vertex3le = leftMesh->newVertex();
  leftMesh->m_vertices->setLocalPos(vertex0le, p1l);
  leftMesh->m_vertices->setLocalPos(vertex1le, p3l);
  leftMesh->m_vertices->setLocalPos(vertex2le, p1u);
  leftMesh->m_vertices->setLocalPos(vertex3le, p3u);
  leftMesh->newTriangle(vertex0le, vertex1le, vertex2le);
  leftMesh->newTriangle(vertex3le, vertex2le, vertex1le);
  leftMesh->computeAllNormals();
  leftMesh->computeBoundaryBox(true);
  leftMesh->createAABBCollisionDetector(toolRadius);
  leftMesh->setStiffness(0.08*stiffness);
  leftMesh->setShowTriangles(false);
  leftMesh->m_material->setHapticTriangleSides(false, true);
  
  rightMesh = new cMesh();
  rightMesh->setLocalPos(0.0, 0.0, 0.0);
  rightMesh->setUseCulling(false);
  int vertex0r = rightMesh->newVertex();
  int vertex1r = rightMesh->newVertex();
  int vertex2r = rightMesh->newVertex();
  int vertex3r = rightMesh->newVertex();
  rightMesh->m_vertices->setLocalPos(vertex0r, p0l);
  rightMesh->m_vertices->setLocalPos(vertex1r, p2l);
  rightMesh->m_vertices->setLocalPos(vertex2r, p0u);
  rightMesh->m_vertices->setLocalPos(vertex3r, p2u);
  rightMesh->newTriangle(vertex0r, vertex1r, vertex2r);
  rightMesh->newTriangle(vertex3r, vertex2r, vertex1r);
  rightMesh->computeAllNormals();
  rightMesh->computeBoundaryBox(true);
  rightMesh->createAABBCollisionDetector(toolRadius);
  rightMesh->setStiffness(0.08*stiffness);
  rightMesh->setShowTriangles(false);
  rightMesh->m_material->setHapticTriangleSides(true, false);


}
