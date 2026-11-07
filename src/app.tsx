import { Router } from "@solidjs/router";
import { FileRoutes } from "@solidjs/start/router";
import { Suspense } from "solid-js";
import Nav from "~/components/ui/Nav";
import "./app.css";
import CrachaForm from "./features/cracha/CrachaForm";

export default function App() {
  return (
    <CrachaForm onSubmit={()=>{}}/>
    // <Router
    //   root={(props) => (
    //     <>
    //       <Nav />
    //       <Suspense>{props.children}</Suspense>
    //     </>
    //   )}
    // >
    //   <FileRoutes />
    // </Router>
  );
}
