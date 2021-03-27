Things and How to Interact With Them
====================================

Overview
^^^^^^^^

    sundile's memory handling is performed by the excellent
    EnTT. All of the library's functionality is exposed
    to the end user. You may refer to `EnTT's documentation <https://entt.docsforge.com/>`_
    for more information on how to use the ECS.

    sundile's event system also relies on EnTT.

Entities, Components, Systems
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    Entities contain components. Systems view components
    and perform functions on them. Components themselves
    are just packets of data.

    Entities are represented with an index into a registry.
    Systems are namespaces containing functions.
    Components are simple structs.

    Although there is no restriction, Components should
    only contain fields, while Systems should strive
    to only contain functions. Sometimes this isn't
    possible, so the flexibility still exists.

    There are a few macros available to make creating
    and using the ECS system easier.

API
^^^

Creating Systems and Components
"""""""""""""""""""""""""""""""

    ==================== ===========================
    macro                functionality
    ==================== ===========================
    ``COMPONENT(_name)`` Begins component definition.
    ``END_COMPONENT``    Ends component definition.
    ``SYSTEM(_name)``    Begins system definition.
    ``END_SYSTEM``       Ends system definition.
    ==================== ===========================

    These macros should all appear at the global scope,
    as they include all the necessary scoping to ensure
    that components and systems remain accessible to
    sundile.

    **example:** ::

        COMPONENT(MyComponent)
            int a, b, c;
        END_COMPONENT

        SYSTEM(MySystem)
            int sum (vec3 vec) {
                return vec.a + vec.b + vec.c;
            }
        END_SYSTEM


Creating Scenes and Entities
""""""""""""""""""""""""""""
    **Creating entities and using systems:**::

        entt::registry registry;
        entt::entity MyEntity = registry->create();
        MyComponent comp = emplace<MyComponent>(registry, MyEntity, 1,2,3);
        MySystem::add(comp);
    
    Note that the ``emplace`` function takes a variable
    number of arguments. This way you can initialize
    your component at creation with no memory overhead.
    You may also, of course, create the component and
    then emplace it with a direct reference.

    Systems are also incredibly simple to use. Simply
    call the system name and it's correlated function,
    and pass it the relevant component.

    **Creating Scenes:**

    A scene is more or less the same thing as a collection
    of entity, component, and system calls.
    [TODO: Better define scene creation.]


Projects and File Structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^

    Projects are saved in the sundile project root directory.
    This is set to ``~/Documents/sundile/`` by default.

    Your project directory should be structured as follows:::

        ~/Documents/sundile/
            project_name/
                assets/
                components/
                    MyComponent.h
                systems/
                    MySystem.h
                project_name.SUNPROJ

    The folder names ought to be pretty straightforward.
    The SUNPROJ file is a compact binary representation
    of all the information for your scenes and the
    entities within them.

    Components and systems are written as C++ header files.
    