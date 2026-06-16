// ===== STATE MANAGEMENT =====
let settings = {
    hotkeys: {
        sprint: 33,
        blink: 45,
        invisible: 34
    },
    toggles: {
        sprint: true,
        blink: true,
        invisible: true
    },
    sprint: {
        duration: 5.0,
        speed: 75.0
    }
};

// ===== BRIDGE COMMUNICATION =====
function sendBridge(data) {
    if (window.EnxyAbilities_Bridge) {
        window.EnxyAbilities_Bridge(JSON.stringify(data));
    } else {
        console.log('[EnxyAbilities] Bridge not available, data:', data);
    }
}

// Sync settings dari C++ plugin (dipanggil oleh PMCM)
window.syncSettings = function(jsonStr) {
    try {
        const data = JSON.parse(jsonStr);
        
        // Update hotkeys
        if (data.iHotkey_Sprint !== undefined) {
            settings.hotkeys.sprint = data.iHotkey_Sprint;
            updateHotkeyDisplay('sprint', data.iHotkey_Sprint);
        }
        if (data.iHotkey_Blink !== undefined) {
            settings.hotkeys.blink = data.iHotkey_Blink;
            updateHotkeyDisplay('blink', data.iHotkey_Blink);
        }
        if (data.iHotkey_Invisible !== undefined) {
            settings.hotkeys.invisible = data.iHotkey_Invisible;
            updateHotkeyDisplay('invisible', data.iHotkey_Invisible);
        }
        
        // Update toggles
        if (data.bEnable_Sprint !== undefined) {
            settings.toggles.sprint = data.bEnable_Sprint;
            document.getElementById('enable-sprint').checked = data.bEnable_Sprint;
        }
        if (data.bEnable_Blink !== undefined) {
            settings.toggles.blink = data.bEnable_Blink;
            document.getElementById('enable-blink').checked = data.bEnable_Blink;
        }
        if (data.bEnable_Invisible !== undefined) {
            settings.toggles.invisible = data.bEnable_Invisible;
            document.getElementById('enable-invisible').checked = data.bEnable_Invisible;
        }
        
        // Update sprint settings
        if (data.fSprint_Duration !== undefined) {
            settings.sprint.duration = data.fSprint_Duration;
            const slider = document.getElementById('sprint-duration');
            slider.value = data.fSprint_Duration;
            updateDurationDisplay(data.fSprint_Duration);
        }
        if (data.fSprint_SpeedMult !== undefined) {
            settings.sprint.speed = data.fSprint_SpeedMult;
            const slider = document.getElementById('sprint-speed');
            slider.value = data.fSprint_SpeedMult;
            updateSpeedDisplay(data.fSprint_SpeedMult);
        }
        
        console.log('[EnxyAbilities] Settings synced:', settings);
    } catch (e) {
        console.error('[EnxyAbilities] syncSettings parse error:', e);
    }
};

// ===== HOTKEY HANDLING =====
const hotkeyInputs = {
    sprint: document.getElementById('hotkey-sprint'),
    blink: document.getElementById('hotkey-blink'),
    invisible: document.getElementById('hotkey-invisible')
};

function updateHotkeyDisplay(skill, keyCode) {
    const keyName = getKeyName(keyCode);
    hotkeyInputs[skill].value = keyName;
}

function getKeyName(keyCode) {
    const keyMap = {
        33: 'F', 34: 'G', 45: 'X', 46: 'DELETE',
        16: 'SHIFT', 17: 'CTRL', 18: 'ALT',
        32: 'SPACE', 13: 'ENTER', 27: 'ESC'
    };
    return keyMap[keyCode] || `KEY ${keyCode}`;
}

function setupHotkeyCapture() {
    Object.entries(hotkeyInputs).forEach(([skill, input]) => {
        let isListening = false;
        
        input.addEventListener('click', function() {
            if (isListening) return;
            
            this.value = 'Press any key...';
            this.classList.add('listening');
            isListening = true;
            
            const listener = (e) => {
                e.preventDefault();
                const keyCode = e.keyCode || e.which;
                
                // Update UI
                this.value = getKeyName(keyCode);
                this.classList.remove('listening');
                isListening = false;
                
                // Update state
                settings.hotkeys[skill] = keyCode;
                
                // Send to bridge (live update)
                sendBridge({
                    action: 'liveHotkey',
                    skill: skill,
                    keyCode: keyCode
                });
                
                document.removeEventListener('keydown', listener);
                console.log(`[EnxyAbilities] Hotkey set: ${skill} = ${keyCode}`);
            };
            
            document.addEventListener('keydown', listener);
        });
    });
}

// ===== TOGGLE HANDLING =====
function setupToggles() {
    const toggles = {
        sprint: document.getElementById('enable-sprint'),
        blink: document.getElementById('enable-blink'),
        invisible: document.getElementById('enable-invisible')
    };
    
    Object.entries(toggles).forEach(([skill, toggle]) => {
        toggle.addEventListener('change', function() {
            settings.toggles[skill] = this.checked;
            
            sendBridge({
                action: 'liveToggle',
                skill: skill,
                enabled: this.checked
            });
            
            console.log(`[EnxyAbilities] ${skill} enabled: ${this.checked}`);
        });
    });
}

// ===== SLIDER HANDLING =====
const durationSlider = document.getElementById('sprint-duration');
const speedSlider = document.getElementById('sprint-speed');
const durationVal = document.getElementById('duration-val');
const speedVal = document.getElementById('speed-val');
const previewSpeed = document.getElementById('preview-speed');
const previewDuration = document.getElementById('preview-duration');

function updateDurationDisplay(value) {
    durationVal.textContent = value + 's';
    previewDuration.textContent = value + 's';
}

function updateSpeedDisplay(value) {
    speedVal.textContent = value + '%';
    const totalSpeed = 100 + parseInt(value);
    previewSpeed.textContent = totalSpeed + '%';
}

function setupSliders() {
    durationSlider.addEventListener('input', function() {
        const val = parseFloat(this.value);
        settings.sprint.duration = val;
        updateDurationDisplay(val);
        
        sendBridge({
            action: 'liveSlider',
            setting: 'duration',
            value: val
        });
    });
    
    speedSlider.addEventListener('input', function() {
        const val = parseFloat(this.value);
        settings.sprint.speed = val;
        updateSpeedDisplay(val);
        
        sendBridge({
            action: 'liveSlider',
            setting: 'speed',
            value: val
        });
    });
}

// ===== SAVE/RESET FUNCTIONS =====
function saveSettings() {
    sendBridge({
        action: 'save',
        iHotkey_Sprint: settings.hotkeys.sprint,
        iHotkey_Blink: settings.hotkeys.blink,
        iHotkey_Invisible: settings.hotkeys.invisible,
        bEnable_Sprint: settings.toggles.sprint,
        bEnable_Blink: settings.toggles.blink,
        bEnable_Invisible: settings.toggles.invisible,
        fSprint_Duration: settings.sprint.duration,
        fSprint_SpeedMult: settings.sprint.speed
    });
    
    console.log('[EnxyAbilities] Settings saved:', settings);
}

function resetSettings() {
    // Reset ke default
    const defaults = {
        hotkeys: { sprint: 33, blink: 45, invisible: 34 },
        toggles: { sprint: true, blink: true, invisible: true },
        sprint: { duration: 5.0, speed: 75.0 }
    };
    
    // Update state
    settings = JSON.parse(JSON.stringify(defaults));
    
    // Update UI
    updateHotkeyDisplay('sprint', defaults.hotkeys.sprint);
    updateHotkeyDisplay('blink', defaults.hotkeys.blink);
    updateHotkeyDisplay('invisible', defaults.hotkeys.invisible);
    
    document.getElementById('enable-sprint').checked = defaults.toggles.sprint;
    document.getElementById('enable-blink').checked = defaults.toggles.blink;
    document.getElementById('enable-invisible').checked = defaults.toggles.invisible;
    
    durationSlider.value = defaults.sprint.duration;
    speedSlider.value = defaults.sprint.speed;
    updateDurationDisplay(defaults.sprint.duration);
    updateSpeedDisplay(defaults.sprint.speed);
    
    // Send to bridge
    sendBridge({ action: 'reset' });
    
    console.log('[EnxyAbilities] Settings reset to default');
}

// ===== INITIALIZATION =====
function initSettings() {
    // Setup event listeners
    setupHotkeyCapture();
    setupToggles();
    setupSliders();
    
    // Initialize displays
    updateHotkeyDisplay('sprint', settings.hotkeys.sprint);
    updateHotkeyDisplay('blink', settings.hotkeys.blink);
    updateHotkeyDisplay('invisible', settings.hotkeys.invisible);
    updateDurationDisplay(settings.sprint.duration);
    updateSpeedDisplay(settings.sprint.speed);
    
    console.log('[EnxyAbilities] UI initialized');
}

// Init saat DOM ready
document.addEventListener('DOMContentLoaded', initSettings);