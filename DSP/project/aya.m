% Load files css.mat
load('css.mat');

% Repeat the CSS sequence ten times as the far-end signal
numBlocks = 10;
farEndSignal = repmat(css, numBlocks, 1);

% Reshape far-end signal and path to column vectors
farEndSignal = farEndSignal(:);
path = path(:);

% Generate the echo signal
echoSignal = conv(farEndSignal, path); % Convolve far-end signal with impulse response

% Initialize adaptive filter parameters
filterLength = 128;
w = zeros(filterLength, 1); % Adaptive filter weights
mu = 0.25; % Step size parameter
epsilon = 1e-6; % Small constant to avoid division by zero

% Perform adaptive filtering using NLMS algorithm
echoEstimate = zeros(size(echoSignal));
errorSignal = zeros(size(echoSignal));

for n = filterLength:length(echoSignal)
    x = farEndSignal(n:-1:n-filterLength+1);
    y = w' * x;
    e = echoSignal(n) - y;
    
    w = w + (mu / (epsilon + norm(x)^2)) * conj(x) * e;
    
    echoEstimate(n) = y;
    errorSignal(n) = e;
end

% Plot the signals and echo path estimate
figure;
subplot(4,1,1);
plot(farEndSignal);
title('Far-End Signal');
xlabel('Sample');
ylabel('Amplitude');

subplot(4,1,2);
plot(echoSignal);
title('Echo Signal');
xlabel('Sample');
ylabel('Amplitude');

subplot(4,1,3);
plot(errorSignal);
title('Error Signal');
xlabel('Sample');
ylabel('Amplitude');

subplot(4,1,4);
plot(echoSignal, 'b', 'DisplayName', 'Echo Signal');
hold on;
plot(echoEstimate, 'r', 'DisplayName', 'Echo Estimate');
hold off;
title('Echo Path and Estimate');
xlabel('Sample');
ylabel('Amplitude');
legend('Location', 'best');

% Display the completion message
fprintf('Adaptive Line Echo Canceller Simulation Completed.\n');
