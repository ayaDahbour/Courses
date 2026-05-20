clc;
clear;
close all;

% Define transfer function G(s) = 2 / (s*(s+3))
num = 2;
den = conv([1 0], [1 3]);  % s*(s+3) = s^2 + 3s

% Create transfer function
G = tf(num, den);

% Create unity feedback system
sys = feedback(G, 1);

% Plot step response
figure;
step(sys);
title('Step Response of Unity Feedback System');
xlabel('Time (sec)');
ylabel('Amplitude');
grid on;

% Display poles for verification
disp('Poles of the closed-loop system:');
disp(pole(sys));
