import { openDB } from "idb";

let dbPromise = null;

export function getDB(){
  if (typeof indexedDB === "undefined") {
    return null; // SSR
  }

  if(!dbPromise){
    dbPromise = openDB('appDB', 1, {
      upgrade(db){
        //livro ponto
        if (!db.objectStoreNames.contains('lp-book-info')) {
          const store = db.createObjectStore('lp-book-info', { keyPath:"id", autoIncrement: true });
          store.createIndex("ume", "ume", {unique: false});
          store.createIndex("refType", "refType", {unique: false});
          store.createIndex("name", "name", {unique: false});
        }
        if (!db.objectStoreNames.contains('lp-page')) {
          db.createObjectStore('lp-page', {keyPath: "pageNum"});
        }
        //cracha
        if (!db.objectStoreNames.contains('cracha-info')){
          const store = db.createObjectStore('cracha-info', { keyPath:"id", autoIncrement: true });
          store.createIndex('group', ['ume', 'refType', 'refYear', 'refClass']);
        }
      }
    });
  }

  return dbPromise;
}