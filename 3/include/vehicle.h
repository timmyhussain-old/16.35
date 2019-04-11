#ifndef __VEHICLE_H__
#define __VEHICLE_H__

struct t_simulator; // forward declaration
struct t_control;
typedef struct t_vehicle {
    double position [3]; // x,y,theta
    double velocity [3]; // dx,dy,dtheta
    void * control_data; // for use by relevant controller.
    struct t_simulator * simulator;
    // vehicle function calls
    struct t_control (*get_control)(struct t_vehicle * v);          // applies appropriate control rule
    void (*set_position)   (struct t_vehicle * v, double * values); // applies constraints and updates values.
    void (*set_velocity)   (struct t_vehicle * v, double * values); // applies constraints and updates values.
    void (*control_vehicle)(struct t_vehicle * v);                  // calculates and applies controls to vehicle
    void (*update_state)   (struct t_vehicle * v, double time);     // updates vehicle states based on dynamics
    void (*run) (struct t_vehicle * v);                             // runs the vehicle update loop
} vehicle;

// create vehicle
vehicle * create_vehicle(double * starting_position, struct t_simulator * sim);
// standard vehicle methods
void set_position   (struct t_vehicle * v,double * values);
void set_velocity   (struct t_vehicle * v,double * values);
void control_vehicle(struct t_vehicle * v);
void update_state   (struct t_vehicle * v, double time);
void vehicle_run (struct t_vehicle * v);
#endif
