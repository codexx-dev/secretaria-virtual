import Cat from "../components/misc/Cat";
import { pickRandom } from "../utils";

const messages = [
  "Sou eu, o cara que programa...😪",
  "👆Clique no gato AGORA.",
]

function About(){
  return(
    <div className="grow h-full py-20 text-neutral-50 flex flex-col gap-10 overflow-y-scroll">
      <h1 className="text-center text-4xl font-semibold">SOBRE</h1>
      <Cat/>
      <p className="text-3xl text-center">{pickRandom(messages)}</p>
    </div>
  );
}

export default About;