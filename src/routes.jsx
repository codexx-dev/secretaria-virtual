//import { createBrowserRouter } from "react-router-dom";
import Home from "./pages/Home";
import About from "./pages/About";
import Page404 from "./pages/Page404";
import WorkInProgress from "./pages/WorkInProgress";
import LPEditor from "./features/livro-ponto/LPEditor";
import LPCreator from "./features/livro-ponto/LPCreator";
import AppLayout from "./layouts/AppLayout";
import CrachaCreator from "./features/cracha/CrachaCreator";
import CrachaPrint from "./features/cracha/CrachaPint";
import PrintLayout from "./layouts/PrintLayout";

export const BASENAME = 'secretaria-virtual/';

export const routes = [
  {
    path: '/',
    element: <AppLayout/>,
    children:[
      {index: true, element: <Home/>},
      { path: '/sobre', element: <About/> },
      { path: '/livro-ponto', element: <LPCreator/> },
      { path: '/livro-ponto/editor', element: <LPEditor/> },
      { path: '/cracha', element: <CrachaCreator/>},
      { path: '/em-progresso', element: <WorkInProgress/> }
    ]
  },
  {
    path: '/imprimir',
    element: <PrintLayout/>,
    children:[
      { path: '/imprimir/cracha', element: <CrachaPrint/> }
    ]
  },
  { path: '*', element: <Page404/> }
];

// export const router = createBrowserRouter(
//   routes,
//   {basename: '/secretaria-virtual'}
// );