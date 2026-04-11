import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, CheckButtons
from scipy import signal

class Harmonic:
    def __init__(self):
        self.t = np.linspace(0, 20, 1000)
        self.fs = 100.0
        self.amplitude = 1.0
        self.frequency = 0.25
        self.phase = 0.0
        self.noise_mean = 0.0
        self.noise_covariance = 0.1
        self.cutoff = 2.0
        self.current_mean = self.noise_mean
        self.current_cov = self.noise_covariance
        self.noise = self.generate_noise(self.current_mean, self.current_cov)
        self.setup_ui()

    def generate_noise(self, mean, cov):
        std_dev = np.sqrt(max(cov, 0.0001))
        return np.random.normal(mean, std_dev, len(self.t))

    def calc_harmonic(self, amp, freq, phase):
        return amp * np.sin(2 * np.pi * freq * self.t + phase)

    def filter_signal(self, noisy_signal, cutoff):
        nyq = 0.5 * self.fs
        normal_cutoff = cutoff / nyq
        if normal_cutoff >= 1.0:
            normal_cutoff = 0.99
        b, a = signal.butter(3, normal_cutoff, btype='low', analog=False)
        return signal.filtfilt(b, a, noisy_signal)

    def setup_ui(self):
        self.fig, self.ax = plt.subplots(figsize=(10, 8))
        plt.subplots_adjust(bottom=0.45)
        y_clean = self.calc_harmonic(self.amplitude, self.frequency, self.phase)
        y_noisy = y_clean + self.noise
        y_filtered = self.filter_signal(y_noisy, self.cutoff)
        self.line_noisy, = self.ax.plot(self.t, y_noisy, color='orange', alpha=0.8, label='Шум')
        self.line_clean, = self.ax.plot(self.t, y_clean, color='blue', linestyle='--', linewidth=2, label='Чиста')
        self.line_filtered, = self.ax.plot(self.t, y_filtered, color='purple', linewidth=2, label='Відфільтрована')
        self.ax.set_title('Графік гармоніки')
        self.ax.set_ylim(-3, 3)
        self.ax.legend(loc='upper right')

        ax_amp = plt.axes([0.25, 0.35, 0.65, 0.03])
        ax_freq = plt.axes([0.25, 0.30, 0.65, 0.03])
        ax_phase = plt.axes([0.25, 0.25, 0.65, 0.03])
        ax_mean = plt.axes([0.25, 0.20, 0.65, 0.03])
        ax_cov = plt.axes([0.25, 0.15, 0.65, 0.03])
        ax_cutoff = plt.axes([0.25, 0.10, 0.65, 0.03])
        
        self.s_amp = Slider(ax_amp, 'Amplitude', 0.1, 3.0, valinit=self.amplitude)
        self.s_freq = Slider(ax_freq, 'Frequency', 0.1, 5.0, valinit=self.frequency)
        self.s_phase = Slider(ax_phase, 'Phase', 0.0, 2*np.pi, valinit=self.phase)
        self.s_mean = Slider(ax_mean, 'Noise Mean', -1.0, 1.0, valinit=self.noise_mean)
        self.s_cov = Slider(ax_cov, 'Noise Covariance', 0.0, 1.0, valinit=self.noise_covariance)
        self.s_cutoff = Slider(ax_cutoff, 'Cutoff Frequency', 0.1, 20.0, valinit=self.cutoff)
        self.s_amp.on_changed(self.update)
        self.s_freq.on_changed(self.update)
        self.s_phase.on_changed(self.update)
        self.s_mean.on_changed(self.update)
        self.s_cov.on_changed(self.update)
        self.s_cutoff.on_changed(self.update)
        ax_reset = plt.axes([0.1, 0.02, 0.1, 0.05])
        self.btn_reset = Button(ax_reset, 'Reset')
        self.btn_reset.on_clicked(self.reset)
        ax_check = plt.axes([0.8, 0.02, 0.15, 0.05])
        self.chk_noise = CheckButtons(ax_check, ['Show Noise'], [True])
        self.chk_noise.on_clicked(self.toggle_noise)
        plt.show()

    def update(self, val):
        if self.s_mean.val != self.current_mean or self.s_cov.val != self.current_cov:
            self.noise = self.generate_noise(self.s_mean.val, self.s_cov.val)
            self.current_mean = self.s_mean.val
            self.current_cov = self.s_cov.val
        y_clean = self.calc_harmonic(self.s_amp.val, self.s_freq.val, self.s_phase.val)
        y_noisy = y_clean + self.noise
        y_filtered = self.filter_signal(y_noisy, self.s_cutoff.val)
        self.line_clean.set_ydata(y_clean)
        self.line_noisy.set_ydata(y_noisy)
        self.line_filtered.set_ydata(y_filtered)
        self.fig.canvas.draw_idle()
    def reset(self, event):
        self.s_amp.reset()
        self.s_freq.reset()
        self.s_phase.reset()
        self.s_mean.reset()
        self.s_cov.reset()
        self.s_cutoff.reset()
    def toggle_noise(self, label):
        show_n = self.chk_noise.get_status()[0]
        self.line_noisy.set_visible(show_n)
        self.fig.canvas.draw_idle()
if __name__ == '__main__':
    app = Harmonic()