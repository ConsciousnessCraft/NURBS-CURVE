# NURBS-CURVE
Non-uniform rational B-spline Curve
Definition: A Non-Uniform Rational B-Spline (NURBS) curve is a mathematical representation used in computer graphics, CAD (Computer-Aided Design), and computer-aided geometric design to create and represent curves and surfaces. NURBS are an extension of B-spline curves and allow for greater flexibility and precision in modeling complex shapes.
Key Characteristics:

Control Points:

NURBS curves are defined by a set of control points that determine the shape of the curve. The curve does not generally pass through all control points; rather, they influence the curve's form.
Weights:

Each control point has an associated weight that influences its effect on the curve. This weight allows for the representation of conic sections (like circles and ellipses) and other complex shapes more accurately than traditional B-splines.
Non-Uniform:

The term "non-uniform" refers to the fact that the spacing between the knot values (parameters that define the curve's domain) does not need to be uniform. This allows for localized control over the curve's shape.
Knot Vector:

A knot vector is a sequence of parameter values that partitions the parameter space of the curve. It determines how the control points influence the curve and affects continuity and smoothness.
Polynomial Basis Functions:

NURBS curves are defined using piecewise polynomial functions called basis functions, which are non-negative and sum to one over the domain.
Mathematical Representation: The NURBS curve 𝐶(𝑢)
C(u) can be defined as:

The NURBS curve \( C(u) \) can be defined as:

<p>
C(u) = <sup>∑<sub>i=0</sub><sup>n</sup></sup> N<sub>i,p</sub>(u) ⋅ w<sub>i</sub> ⋅ P<sub>i</sub> 
</p>

<p>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; / <sup>∑<sub>i=0</sub><sup>n</sup></sup> N<sub>i,p</sub>(u) ⋅ w<sub>i</sub>
</p>



Applications:

Computer Graphics: NURBS are widely used to model complex curves and surfaces in 3D graphics, animations, and rendering.
CAD/CAM: They are essential in creating precise geometric models for engineering and manufacturing applications.
Animation: NURBS curves can define motion paths for objects in animations.
Advantages:

NURBS can represent a wide range of geometric shapes, including lines, circles, and complex curves, with high precision.
The ability to adjust weights and control points allows for intuitive manipulation of shapes.
Summary
NURBS curves are a powerful tool in geometric modeling, offering flexibility and precision for a variety of applications. They are fundamental in fields like computer graphics, CAD, and animations due to their capability to accurately represent complex shapes.


