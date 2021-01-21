![Compile Debug](https://github.com/Autoquark/SdlBreakout/workflows/Compile%20Debug/badge.svg)
![Compile Release](https://github.com/Autoquark/SdlBreakout/workflows/Compile%20Release/badge.svg)
![Unit Tests](https://github.com/Autoquark/SdlBreakout/workflows/Unit%20Tests/badge.svg)
![Cppcheck](https://github.com/Autoquark/SdlBreakout/workflows/cppcheck/badge.svg)

# SDL Breakout

SDL Breakout is a breakout game written in C++ using Simple DirectMedia Layer, a C/C++ library that provides low level graphics, input and sound access.

It includes:

- A from-scratch 2D collision detection system
- A modular system for powerups and ball behaviour modifications
- Unit tests

It was written as a C++ learning excercise, so the primary focus was on code with a minimum of flashiness or gameplay polish.

The collision detection system is implemented via a double-dispatch pattern. There is a hierarchy of types representing various primitive shapes all inheriting from an abstract Shape base class; Shape defines a CastAgainst(Shape other, ...) method which sweeps along a given movement path for collisions with a given other shape, and a family of CastAgainstThis(T other, ...) methods, one for each Shape subclass. Subclasses override CastAgainst to call the appropriate CastAgainstThis overload on the other shape, resulting in a call to a method that knows the specific type of both shapes.

Two more advanced concepts are built in top of this system; the first is the CompoundShape class, which represents a shape defined as the union of multiple other shapes. This class implements all collision detection methods simply by delegating the call to all its constituent shapes and then filtering the resulting values; this means that the other shape classes do not need to know about CompoundShape or implement any specific logic relating to it.
The second is Polygon which allows for representing any shape via a sequence of points representing the vertices. These are converted into a set of lines and collision detection is delegated to the existing logic in the Line class similarly to how CompoundShape is implemented. Theoretically Polygon could be used in place of most of the other Shape subclasses, but it is less performant and lacks various specialized methods that make working with specific shape subclasses more convenient and readable.

The various powerups and effects that can modify the behaviour of a ball are implemented via a modular system that is designed to be extensible. Ball statuses are represented by classes deriving from BallStatusEffect. A block that applies a status to the ball when hit holds a pointer to an instance of such a class. When the ball hits the block, the status' ApplyToBall method is called which in most cases clones the status and adds the clone to the ball's list of active statuses. These active statuses are notified via a set of callback methods whenever certain events occurs, such as the ball hitting the paddle or a block, allowing for a variety of behaviours to be implemented.

I attempted to follow best practices and make use of modern C++ features in order to make the code as readable and maintainable as possible. Most dynamic memory allocations are handled via std::unique_ptr. I made use of std::optional in order to avoid dynamic memory allocations when returning possibly 'empty' values. Unit tests are included, mostly focused around the collision detection code including regression tests for various issues I encountered, often relating to floating-point rounding error.
