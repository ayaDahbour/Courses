clc;
clear;
close all;

%% Define the system
num = 9;
den = conv([1 0], [1 2]); % s(s+2) = s^2 + 2s

G = tf(num, den);

% Closed-loop system with unity feedback
sys = feedback(G, 1);

%% Plot Step Response
figure;
step(sys);
title('Step Response');
xlabel('Time (sec)');
ylabel('Amplitude');
grid on;

%% Step Response Characteristics
info = stepinfo(sys);

fprintf('Rise Time (Tr): %.4f sec\n', info.RiseTime);
fprintf('Settling Time (Ts): %.4f sec\n', info.SettlingTime);
fprintf('Peak Time (Tp): %.4f sec\n', info.PeakTime);
fprintf('Overshoot (OS): %.2f%%\n', info.Overshoot);

%% Poles, damping ratio and natural frequency
poles = pole(sys);
disp('Poles:');
disp(poles);

wn = abs(poles(1));  % natural frequency
zeta = -real(poles(1))/wn;  % damping ratio

fprintf('Natural Frequency (wn): %.4f rad/s\n', wn);
fprintf('Damping Ratio (zeta): %.4f\n', zeta);
