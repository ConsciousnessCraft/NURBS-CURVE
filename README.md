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
The NURBS curve C(u) can be defined as:

C(u) = ∑<sub>i=0</sub><sup>n</sup> N<sub>i,p</sub>(u) ⋅ w<sub>i</sub> ⋅ P<sub>i</sub> 
/ ∑<sub>i=0</sub><sup>n</sup> N<sub>i,p</sub>(u) ⋅ w<sub>i</sub>




