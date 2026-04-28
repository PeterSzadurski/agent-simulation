
import { useState, useRef } from 'react';
import SimulationCanvas from './canvas'
import {  Grid } from '@mui/material';
import { ThemeProvider, createTheme } from '@mui/material/styles';
import CssBaseline from '@mui/material/CssBaseline';
import SimStats from './SimStats';
import SimStatsPanel from './SimStatsPanel';
import EntityStats from './EntityStats';
import EntityStatsPanel from './EntityStatsPanel';
import TickCounter from './TickCounter';

const darkTheme = createTheme({
  palette: {
    mode: 'dark',
  },
});


function App() {

    const [tick, setTick] = useState(0);
    const [speed, setSpeed] = useState(1);
    const [simStats, setSimStats] = useState<SimStats>(new SimStats());
    const [entityStats, setEntityStats] = useState<EntityStats>(new EntityStats);
    const [hasSelectedEntity, setSelectedEntity] = useState(false);

  return (
    <ThemeProvider theme={darkTheme}>
      <CssBaseline />
      <Grid container spacing={1}>
        <Grid size={4}>
          <SimStatsPanel simStats={simStats}></SimStatsPanel>
        </Grid>
        <Grid size={4}>
          <SimulationCanvas onTick={setTick} onSimStats={setSimStats} onEntityStats={setEntityStats} onSelectedEntity={setSelectedEntity} hasSelectedEntity={hasSelectedEntity}/>
          <TickCounter tick={tick}></TickCounter>
        </Grid>
        <Grid size={4}>
          <EntityStatsPanel entityStats={entityStats}></EntityStatsPanel>
        </Grid>
      </Grid>
    </ThemeProvider>
  )
}

export default App
