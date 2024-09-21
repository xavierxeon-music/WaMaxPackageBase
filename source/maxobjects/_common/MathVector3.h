#ifndef MathVector3H
#define MathVector3H

namespace Math
{
   struct Spherical
   {
      double az = 0.0;
      double el = 0.0;
      double radius = 0.0;

      bool operator<(const Spherical& other) const;
   };

   class Vector3
   {
   public:
      Vector3(const double& x = 0.0, const double& y = 0.0, const double& z = 0.0);

   public:
      bool operator==(const Vector3& other) const;
      bool operator<(const Vector3& other) const;
      const double& operator[](const int index) const;
      double& operator[](const int index);

   public:
      static Vector3 fromSpherical(const Spherical& spherical, const bool fromDegree = true);
      Spherical toSpherical(const bool toDegree = true) const;

      double length() const;
      Vector3 norm() const;

      double dot(const Vector3& other) const;
      double dotAngle(const Vector3& other, const bool toDegree = true) const;

      Vector3 cross(const Vector3& other) const;
      double crossAngle(const Vector3& other, const bool toDegree = true) const;

   private:
      union
      {
         struct
         {
            double x;
            double y;
            double z;
         };
         double data[3];
      };
   };
} // namespace Math

#ifndef MathVector3HPP
#include "MathVector3.hpp"
#endif // NOT MathVector3HPP

#endif // NOT LinalgH
