Projects and Serialization
==========================

Project Hierarchy
^^^^^^^^^^^^^^^^^

  Containment hierarchy:
    * Project
    * Scene
    * Registry
    * Entity
    * Component
    * Fields

    Projects contain Scenes and take care of serializing them.
    Scenes contain Registries and render them to the screen.
    Registries are the meat of the program. These contain entities and determine what happens to them.
    Entities are simple IDs to which a number of objects are attached.
    Components are simple structs.
    Fields are the members of components, but are conceptually treated as a subclass for GUI purposes.
