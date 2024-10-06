# **NURBS-CURVE**

## **Non-uniform Rational B-spline Curve**

**Definition:** A Non-Uniform Rational B-Spline (NURBS) curve is a mathematical representation used in computer graphics, CAD (Computer-Aided Design), and computer-aided geometric design to create and represent curves and surfaces. NURBS are an extension of B-spline curves and allow for greater flexibility and precision in modeling complex shapes.

## **Key Characteristics:**

### **Control Points:**

NURBS curves are defined by a set of control points that determine the shape of the curve. The curve does not generally pass through all control points; rather, they influence the curve's form.

### **Weights:**

Each control point has an associated weight that influences its effect on the curve. This weight allows for the representation of conic sections (like circles and ellipses) and other complex shapes more accurately than traditional B-splines.

### **Non-Uniform:**

The term "non-uniform" refers to the fact that the spacing between the knot values (parameters that define the curve's domain) does not need to be uniform. This allows for localized control over the curve's shape.

### **Knot Vector:**

A knot vector is a sequence of parameter values that partitions the parameter space of the curve. It determines how the control points influence the curve and affects continuity and smoothness.

### **Polynomial Basis Functions:**

NURBS curves are defined using piecewise polynomial functions called basis functions, which are non-negative and sum to one over the domain.

The NURBS curve **C(u)** can be defined as:

**C(u) = ∑<sub>i=0</sub><sup>n</sup> N<sub>i,p</sub>(u) ⋅ w<sub>i</sub> ⋅ P<sub>i</sub> / ∑<sub>i=0</sub><sup>n</sup> N<sub>i,p</sub>(u) ⋅ w<sub>i</sub>**


## Execution Instructions

To compile and run the project, follow these steps:

### Build the Project

1. Open your terminal and navigate to the project directory.
2. Run the following command to compile the project: **make**
3. After the build process completes, execute the following command to run the application: **./main**

### Controls and Views

Initial View: Displays the curve and its control polygon.

Toggle Frenet Frame and Osculating Circle: Press v to display the Frenet frame and osculating circle.

Add Curvature Radius: Press b to add the curvature radius (represented by the yellow line) to the display along with the Frenet frame and osculating circle.

Move Frenet Along the Curve: Use - and + keys to move the Frenet frame along the curve.

Show Curve and Surface Balayage: Press n to display the curve and the balayage surface.

Return to Initial View: Press c to return to the first view.
 




