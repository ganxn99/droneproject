/*! \mainpage Drone Simulation System Mainpage
 *
 * \tableofcontents
 *
 * \section intro_sec Introduction
 *
 * This project as a whole, though particularly iteration 1, focuses on developing a working foundation for a complete
 * drone delivery simulation, including thorough UML and written documentation as well as
 * functioning code to bring our solutions to life. The motiivating force behind this project is the desire to create a flexible
 * simulation of autonomous drone delivery, so that future real-world implementations have a basis
 * from which they can successfully be built off of.
 *
 * In iteration 2 the foundation developed in iteration 1 was built upon, developing a system with
 * robust drone and package observers, shortest path routing using Dijkstra's, a pool or various
 * drone types, as well as enhanced drone physics, delivery, and scheduling functionality.
 *
 * \section overview_sec Overview
 *
 * \subsection iteration1_subsec Iteration 1
 *
 * The basic technical flow of the simulation as implemented in iteration 1 was as follows:
 * - Drones, Packages, and Customer "entity" objects were generated using a concrete factory. (See csci3081::Drone, csci3081::Package, csci3081::Customer, and csci3081::EntityFactory)\n
 * - These "entities" could then be added to the simulation and displayed in realtime 3D through a
 *  web browser.\n
 * - At any time, a package could be scheduled to be delivered to a certain customer. The simulation
 *  managed this scheduling by commanding the nearest drone to the package to go and pick it up. (If no
 *  drones were currently available, the drone who had the shortest total distance to travel to reach
 *  the package, given all of that drone's currently scheduled/queued deliveries, was chosen).\n
 * - Once a drone had been scheduled a package, it moved along a predefined route to it, picking it
 *  up, and then delivering it to the customer.
 *
 * This implementation was actually already flexible enough to
 *  handle any number of drones, packages and customers. Multiple drones could've been scheduled to deliver multiple
 *  packages, where this was made possible by intelligently determining the length of the route the
 *  drone would take to reach the package, so that the drone with the shortest direct route to the package
 *  could be selected.
 *
 * Other notable features of the iteratioon 1 design include the Facade and Factory design patterns utilized.\n
 *  The csci3081::DroneSimulation class acts as a facade for the entire simulation, meaning that in order for
 *  the basic functionaly of the system to be understood and executed, this is actually the only class that
 *  needs to be interacted with. All of the working parts of the simulation are concealed behind it.\n
 *  The csci3081::EntityFactory class acts as a factory for creating entities in the simulation. The
 *  implementation used here is that of the very basic concrete factory pattern, but it is all that
 *  is necessary for the simplicity of this current implementation.
 *
 * \subsection iteration2_subsec Iteration 2
 *
 * Iteration 2 builds off of the solid foundation developed in iterataion 1, while also expanding it as follows:
 * - Observers can now be added and removed to both packages and drones, allowing for real-time feedback to be displayed as the simulation progresses.
 * - Drones now follow the shortest path to their destinations by using Dijkstra's algorithm, which has been optomized so that routing changes/updates can
 *  be made on the fly as drones are scheduled to pick up new packages during the simulation.
 * - A diverse pool of drone models has been deployed, allowing for a variety of speeds, weights, battery capacities, weight capacities and so on, adding a further element of depth to the simulation.
 * - Drones now follow the limitations provided to them by their model, including the ability to carry multiple packages at once based on their remaining weight capacity, as well as the ability to run out of battery and drop those packages (which will then be rescheduled).
 * - Scheduling has been iterated upon so that drone's are scheduled even more intelligently than before based on a variety of their new characteristics , such as remaining battery and weight capacity.
 * - The drones' package queue has been replaced with a target stack, that allows for much more dynamic runtime routing and re-routing.
 * - Drones now have the ability obey three different physics models: velocity, acceleration, and force. These add depth further still to the simulation.
 *
 * The most notable design pattern of iteration 2 is that of the Observer, implemented here in-part in the csci3081::ObserverManager, which allows for automatic notification when important events occur for either the drone or package entities.
 *
 * \subsection iteration3_subsec Iteration 3
 * In iteration 3, the drone simulation was enhanced one final time through the addition of a variety of design patterns. The goal our team set out to realize with this iteration was that of robustness and extensibility in the design of the simulation, as well as mastery over (and understanding of) the real-world use cases of design patterns in such a system. With this in mind, we initially thought that we would add an array of design patterns to different areas of the simulation without making any major alterations to its functionality (just its extensibility), but in the end the brunt of our design changes were directed at the scheduling and routing algorithms used to direct drones in the simulation. In the end this greatly improved our scheduling and routing to be noticeably more intelligent, and in ways we didn't even originally anticipate! The most notable changes made are as follows:
 * - The concrete factory pattern previously used to create entities was transformed into a composite abstract factory that opens up the project to robust yet simple future extensibility. (See csci3081::EntityFactory, csci3081::DroneFactory, csci3081::PackageFactory, csci3081::CustomerFactory, csci3081::CompositeEntityFactory).
 * - The scheduler class now relies on a composite strategy pattern to intelligently schedule drones, and can utilize different scheduling strategies depending on the user's desires. (See csci3081::DronePool, csci3081::DroneSchedulingStrategy, csci3081::TimeStrategy, csci3081::PotentialDistanceStrategy, csci3081::WeightCapacityStrategy).
 * - The A* pathfinding algorithm was implemented, along with a cache capable of storing previously generated routes for quick lookup, which allowed for the csci3081::TimeStrategy to precisely project the distances of routes the drones would have to traverse to pick up packages, from which the time it would take to traverse those routes could be determined. (See csci3081::AStarNode, csci3081::RouteManager).
 * - The csci3081::RouteManager was converted to a singleton so that its single instance could be accessed anywhere in the program. This became particularly useful when combined with the A* pathfinding and its cache given that the singleton allowed for the cached routes to be accessible from everywhere in the program whenever a route was generated.
 *
 * In short, in iteration 3 we initially set out to implement as many design patterns as we could, sucessfully implementing the abstract composite factory pattern, the strategy pattern, the composite pattern and the singleton pattern, which lead to a noticeably more intelligent scheduling algorithm that, with the help of A*, selects the ideal drone to deliver the package based on the total time it would take for the drone to delivery the package, the potential distance the drone can travel, as well as its weight capacity.
 *
 * \section gettint_started_sec Getting Started
 *
 * To test it out for yourself...\n
 * First, ensure that you are in the `/project` directory.\n
 * Once there, build and run:
 *
 *      make
 *      ./bin/run.sh
 *
 * Then proceed to http://127.0.0.1:8081/ to view the simulation.
 *
 * \section uml_sec UML Diagram
 *
 * The following UML Diagram details the exact interactions between the classes (as developed in this particular implementation)
 *  necessary for this simulation to run.
 *  \image html "Iteration 3 UML - Final.png" "Iteration 3 UML Diagram" width=1800cm
 */
