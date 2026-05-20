% Task 3: PI Controller Design

clc;
clear;

% Given uncompensated settling time
Ts_uncomp = 0.9434;

% Required compensated settling time
Ts_comp = 1.05 * Ts_uncomp;
fprintf('Target compensated settling time: %.4f sec\n', Ts_comp);

% PI controller design (trial values)
Kp = 0.01;
Ki = 0.1;
fprintf('Designed PI controller gains: Kp = %.4f, Ki = %.4f\n', Kp, Ki);
