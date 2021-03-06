/*
 * particle_filter.h
 *
 * 2D particle filter class.
 *  Created on: Dec 12, 2016
 *      Author: Tiffany Huang
 */

#ifndef PARTICLE_FILTER_H_
#define PARTICLE_FILTER_H_

#include "helper_functions.h"

struct Particle {

    int id;
    double x;
    double y;
    double theta;
    double weight;
    std::vector<int> associations;
    std::vector<double> sense_x;
    std::vector<double> sense_y;
};


class ParticleFilter {

protected:

    // Number of particles to draw
    uint num_particles{0};


    // Flag, if filter is initialized
    bool is_initialized{false};

    // Vector of weights of all particles
    std::vector<double> weights;

public:

    // Set of current particles
    std::vector<Particle> particles;

    /**
     * Constructor
     * @param num_particles_ Number of particles
     */
    explicit ParticleFilter(uint num_particles_ = 100);

    // Destructor
    ~ParticleFilter() = default;

    /**
     * init Initializes particle filter by initializing particles to Gaussian
     *   distribution around first position and all the weights to 1.
     * @param x Initial x position [m] (simulated estimate from GPS)
     * @param y Initial y position [m]
     * @param theta Initial orientation [rad]
     * @param std[] Array of dimension 3 [standard deviation of x [m], standard deviation of y [m]
     *   standard deviation of yaw [rad]]
     */
    void init(double x, double y, double theta, double std[]);

    /**
     * prediction Predicts the state for the next time step
     *   using the process model.
     * @param delta_t Time between time step t and t+1 in measurements [s]
     * @param std_pos[] Array of dimension 3 [standard deviation of x [m], standard deviation of y [m]
     *   standard deviation of yaw [rad]]
     * @param velocity Velocity of car from t to t+1 [m/s]
     * @param yaw_rate Yaw rate of car from t to t+1 [rad/s]
     */
    void prediction(double delta_t, double std_pos[], double velocity, double yaw_rate);

    /**
     * dataAssociation Finds which observations correspond to which landmarks (likely by using
     *   a nearest-neighbors data association).
     * @param predicted Vector of predicted landmark observations
     * @param observations Vector of landmark observations
     */
    void dataAssociation(std::vector<LandmarkObs> predicted, std::vector<LandmarkObs> &observations);

    /**
     * updateWeights Updates the weights for each particle based on the likelihood of the
     *   observed measurements.
     * @param sensor_range Range [m] of sensor
     * @param std_landmark[] Array of dimension 2 [Landmark measurement uncertainty [x [m], y [m]]]
     * @param observations Vector of landmark observations
     * @param map Map class containing map landmarks
     */
    void updateWeights(double sensor_range, double std_landmark[], const std::vector<LandmarkObs> &observations,
                       const Map &map_landmarks);

    /**
     * resample Resamples from the updated set of particles to form
     *   the new set of particles.
     */
    void resample();

    /**
     * Finds the landmarks in range of the given coordinates and returns it
     * @param x the x coordinate
     * @param y the y coordinate
     * @param range the range
     * @param landmarks the full set of landmarks
     * @return the landmarks in range
     */
    std::vector<LandmarkObs>
    findLandmarksInRange(double x, double y, double range, std::vector<Map::single_landmark_s> landmarks);

    /**
     * Calculate Multivariate-Gaussian Probability
     * @param prediction the predicted associated landmark
     * @param observation the observerd landmark
     * @param std_landmark Array of dimension 2 [Landmark measurement uncertainty [x [m], y [m]]] (assumed const for optimalization)
     * @return the Multivariate-Gaussian Probability
     */
    double calculateMultivariateGaussianProbability(const LandmarkObs &prediction, const LandmarkObs &observation,
                                                    double std_landmark[]);

    /**
     * Transforms all given coordinates to the reference (x, y, theta)
     * @param observations the list to transform
     * @param x the x reference coordinate
     * @param y the y reference coordinate
     * @param theta the theta of the reference coordinate
     * @return the transformed coordinates
     */
    std::vector<LandmarkObs> transform(std::vector<LandmarkObs> observations, double x, double y, double theta);

    std::string getAssociations(Particle best);

    std::string getSenseX(Particle best);

    std::string getSenseY(Particle best);

    /**
    * initialized Returns whether particle filter is initialized yet or not.
    */
    const bool initialized() const {
        return is_initialized;
    }
};


#endif /* PARTICLE_FILTER_H_ */
