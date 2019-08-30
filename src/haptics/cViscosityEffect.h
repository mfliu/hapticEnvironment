#include "chai3d.h"

using namespace chai3d;
using namespace std;

/**
 * @file cViscosityEffect.h
 * @class cViscosityEffect
 *
 * @brief Creates a velocity-dependent (viscous) field.
 *
 * Apply a velocity-dependent resistive or assistive force to the haptic device. This is done
 * through a 3x3 matrix \f$B\f$
 *
 * \f[
 * B=\begin{bmatrix} 
 * v_x & v_{xy} & v_{xz} \\
 * v_{xy} & v_y & v_{yz} \\
 * v_{xz} & v_{yz} & v_z
 * \end{bmatrix}
 * \f]
 * where each element of \f$B\f$ is a multiplier for the velocity of the haptic tool in that
 * dimension. Thus, resistive forces in the \a x-direction only would have negative values in
 * \f$v_x\f$, while nonzero values in the off-diagonals will produce a curl field.
 */
class cViscosityEffect : public cGenericEffect
{
  public:
    cMatrix3d* viscosityMatrix;

  public:
    cViscosityEffect(cWorld* worldPtr, cMatrix3d* B);
    bool computeForce(const cVector3d& a_toolPos, const cVector3d& a_toolVel,
                      const unsigned int& a_toolID, cVector3d& a_reactionForce);
};
