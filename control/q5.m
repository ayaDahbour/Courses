clc;
clear;
close all;

%% Task 1: Root Locus Plot
disp('--- Root Locus ---');
num = 1;
den = conv(conv([1 2],[1 10]),[1 15]);
G = tf(num, den);
figure;
rlocus(G);
title('Root Locus of G(s)');
grid on;

%% Task 2: Desired Pole and Gain Calculation
disp('--- Desired Pole & Gain ---');
OS_percent = 7;
OS = OS_percent/100;
Ts = 0.9434;

zeta = -log(OS) / sqrt(pi^2 + (log(OS))^2);
wn = 4 / (zeta * Ts);
sigma = zeta * wn;
wd = wn * sqrt(1 - zeta^2);
desired_pole = -sigma + 1j*wd;

fprintf('Damping ratio: %.4f\n', zeta);
fprintf('Natural frequency: %.4f rad/s\n', wn);
fprintf('Desired dominant pole: %.4f %+ .4fj\n', real(desired_pole), imag(desired_pole));

G_at_s = evalfr(G, desired_pole);
K = abs(-1 / G_at_s);
fprintf('Calculated gain K: %.4f\n', K);

%% Task 3: PI Controller Design
disp('--- PI Controller Design ---');
Ts_uncomp = 0.9434;
Ts_comp = 1.05 * Ts_uncomp;
fprintf('Target compensated settling time: %.4f sec\n', Ts_comp);

Kp = 0.01;
Ki = 0.1;
fprintf('Designed PI controller gains: Kp = %.4f, Ki = %.4f\n', Kp, Ki);

%% Task 4: Step Response for both systems
disp('--- Step Responses ---');
G_open = K * G;
sys_uncomp = feedback(G_open, 1);
figure;
step(sys_uncomp);
title('Uncompensated System Step Response');
grid on;

Gc = tf([Kp Ki], [1 0]);
G_comp_open = series(Gc, G_open);
sys_comp = feedback(G_comp_open, 1);
figure;
step(sys_comp);
title('Compensated System Step Response');
grid on;

figure;
step(sys_uncomp, sys_comp);
title('Comparison: Uncompensated vs Compensated');
legend('Uncompensated','Compensated');
grid on;
