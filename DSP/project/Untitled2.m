% Load files css.mat
load('css.mat');
load('path.mat');

% Repeat the CSS sequence ten times as the far-end signal
numBlocks = 10;
Xc = repmat(css, numBlocks, 1); % Concatenate 10 blocks of the composite source signal

% Reshape far-end signal and path to column vectors
Xc = Xc(:);
path = path(:);

% Generate the echo signal
echoSignal = conv(Xc, path); % Convolve far-end signal with impulse response

% Initialize adaptive filter parameters
filterLength = 128;
w = zeros(filterLength, 1); % Adaptive filter weights
mu = 0.25; % Step size parameter
epsilon = 1e-6; % Small constant to avoid division by zero

% Perform adaptive filtering using NLMS algorithm
echoEstimate = zeros(size(echoSignal));
errorSignal = zeros(size(echoSignal));

for n = filterLength:min(length(echoSignal), length(Xc))
    x = Xc(n:-1:n-filterLength+1);
    y = w' * x;
    e = echoSignal(n) - y;
    
    w = w + (mu / (epsilon + norm(x)^2)) * conj(x) * e;
    
    echoEstimate(n) = y;
    errorSignal(n) = e;
end

% Calculate the amplitude and phase response of the estimated FIR channel
H_estimated = freqz(w, 1, 512);
H_given = freqz(path, 1, 512);
amplitude_estimated = 20*log10(abs(H_estimated));
amplitude_given = 20*log10(abs(H_given));
phase_estimated = unwrap(angle(H_estimated));
phase_given = unwrap(angle(H_given));

% Plot the amplitude and phase response
figure;
subplot(2,2,[1,2]);
plot(amplitude_estimated, 'r', 'DisplayName', 'Estimated FIR Channel');
hold on;
plot(amplitude_given, 'b', 'DisplayName', 'Given FIR System (Path)');
hold off;
title('Amplitude Response');
xlabel('Frequency');
ylabel('Amplitude (dB)');
legend('Location', 'best');

subplot(2,2,[3,4]);
plot(phase_estimated, 'r', 'DisplayName', 'Estimated FIR Channel');
hold on;
plot(phase_given, 'b', 'DisplayName', 'Given FIR System (Path)');
hold off;
title('Phase Response');
xlabel('Frequency');
ylabel('Phase (rad)');
legend('Location', 'best');

% Display the completion message
fprintf('Adaptive Line Echo Canceller Simulation Completed.\n');
