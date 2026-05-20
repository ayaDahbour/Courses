clc;
clear;
close all;

%% Define denominator step by step
den1 = conv([1 3], [1 7]);   % (s+3)*(s+7)
den2 = conv([1 9], [1 13]);  % (s+9)*(s+13)
den_total = conv(den1, den2);

% Numerator
num = 1;

% Open-loop transfer function
G = tf(num, den_total);

% Closed-loop transfer function with unity feedback
sys = feedback(G, 1);

% Find poles
poles = pole(sys);
disp('Poles of the closed-loop system:');
disp(poles);
